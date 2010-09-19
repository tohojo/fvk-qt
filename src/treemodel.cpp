/****************************************************************************
 **
 ** Copyright (C) 2005-2007 Trolltech ASA. All rights reserved.
 **
 ** This file is part of the example classes of the Qt Toolkit.
 **
 ** This file may be used under the terms of the GNU General Public
 ** License version 2.0 as published by the Free Software Foundation
 ** and appearing in the file LICENSE.GPL included in the packaging of
 ** this file.  Please review the following information to ensure GNU
 ** General Public Licensing requirements will be met:
 ** http://trolltech.com/products/qt/licenses/licensing/opensource/
 **
 ** If you are unsure which license is appropriate for your use, please
 ** review the following information:
 ** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
 ** or contact the sales department at sales@trolltech.com.
 **
 ** In addition, as a special exception, Trolltech gives you certain
 ** additional rights. These rights are described in the Trolltech GPL
 ** Exception version 1.0, which can be found at
 ** http://www.trolltech.com/products/qt/gplexception/ and in the file
 ** GPL_EXCEPTION.txt in this package.
 **
 ** In addition, as a special exception, Trolltech, as the sole copyright
 ** holder for Qt Designer, grants users of the Qt/Eclipse Integration
 ** plug-in the right for the Qt/Eclipse Integration to link to
 ** functionality provided by Qt Designer and its related libraries.
 **
 ** Trolltech reserves all rights not expressly granted herein.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

/*
   treemodel.cpp

   Provides a simple tree model to show how to create and use hierarchical
   models.
   */

#include <QtGui>
#include <QSqlQuery>

#include "treeitem.h"
#include "treemodel.h"
#include "types.h"

	TreeModel::TreeModel(QObject *parent)
: QAbstractItemModel(parent)
{
	filterDate = true;
	createRoot();
}

void TreeModel::resetData()
{
	delete rootItem;
	createRoot();
	setupModelData();
	reset();
}

void TreeModel::createRoot()
{
	QList<QVariant> rootData;
	rootData << trUtf8("Ting") << trUtf8("Værdi");
	rootItem = new TreeItem(rootData);
}

TreeModel::~TreeModel()
{
	delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
	else
		return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	if(role == Qt::TextAlignmentRole) {
		switch(index.column()) {
			case 0:
				return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
				break;
			case 1:
				return QVariant(Qt::AlignRight | Qt::AlignVCenter);
				break;
		}
	} else if (role != Qt::DisplayRole)
		return QVariant();

	TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

	return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
		int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	if(role == Qt::TextAlignmentRole) {
		switch(section) {
			case 0:
				return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
				break;
			case 1:
				return QVariant(Qt::AlignRight | Qt::AlignVCenter);
				break;
		}
	}

	return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
	const
{
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	TreeItem *parentItem;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
	TreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = rootItem;
	else
		parentItem = static_cast<TreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

QString TreeModel::appendTimeLimit(QString q)
{
	if(filterDate)
		q.append(QString::fromUtf8(" AND time >= %1 AND time <= %2 ").arg(
					startTime.toTime_t()).arg(endTime.toTime_t()));

	return q;
}

void TreeModel::setupModelData()
{
	qDebug() << "TreeModel::setupModelData";
}

TreeItem* TreeModel::addItem(QString text, QString value, TreeItem* parent)
{
	if(!parent)
		parent = rootItem;

	QList<QVariant> cData;
	cData << text;
	cData << value;
	TreeItem* item = new TreeItem(cData, parent);
	parent->appendChild(item);
	return item;
}

TreeItem* TreeModel::addItem(QString text, double value, TreeItem* parent)
{
	if(!parent)
		parent = rootItem;

	QList<QVariant> cData;
	cData << text;
	cData << Types::formatData(value, Types::Currency);
	TreeItem* item = new TreeItem(cData, parent);
	parent->appendChild(item);
	return item;
}

double TreeModel::getDoubleValue(QString q)
{
	q = appendTimeLimit(q);

	qDebug() << q;
	QSqlQuery query(q);
	if(query.next()) {
		return query.value(0).toDouble();
	}
	return 0;
}

void TreeModel::addTop3(QString title, QString q, QString noneFound, QString headerVal)
{
	TreeItem* newItem = addItem(title, headerVal);

	qDebug() << q;
	QSqlQuery query(q);
	while(query.next()) {
		addItem(QString::fromUtf8("%1").arg(query.value(0).toString()), 
				query.value(1).toDouble(), newItem);
	}

	if(!newItem->childCount()) {
		addItem(noneFound, "", newItem);
	}
}
