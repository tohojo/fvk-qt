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

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QDateTime>
#include <QString>

class TreeItem;

class TreeModel : public QAbstractItemModel
{
	Q_OBJECT

	public:
		TreeModel(QObject *parent = 0);
		~TreeModel();

		QVariant data(const QModelIndex &index, int role) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		QVariant headerData(int section, Qt::Orientation orientation,
				int role = Qt::DisplayRole) const;
		QModelIndex index(int row, int column,
				const QModelIndex &parent = QModelIndex()) const;
		QModelIndex parent(const QModelIndex &index) const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;

		virtual void setupModelData();

	private:
		bool filterDate;
		QDateTime startTime;
		QDateTime endTime;

	protected:
		TreeItem *rootItem;
		QString appendTimeLimit(QString q);
		void createRoot();
		void resetData();
		TreeItem* addItem(QString text, QString value, TreeItem* parent = 0);
		TreeItem* addItem(QString text, double value, TreeItem* parent = 0);
		double getDoubleValue(QString query);
		void addTop3(QString title, QString q, QString noneFound, QString headerVal);

	public slots:
		void since_start_changed(int val) {filterDate = (val == 0); resetData();}
		void start_time_changed(QDateTime time) { startTime = time; resetData();}
		void end_time_changed(QDateTime time) { endTime = time; resetData();}
};

#endif
