/**
 * This file is part of NatCafé FVK-system.
 * Copyright (C) 2007 Toke Høiland-Jørgensen
 * 
 * NatCafé FVK-system is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * NatCafé FVK-system is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/
#include <QtSql>
#include "itemsmodel.h"
#include "types.h"

ItemsModel::ItemsModel()
	: QSqlTableModel()
{
	editingEnabled = false;

	setTable("items");
	setEditStrategy(QSqlTableModel::OnManualSubmit);
	setSort(0, Qt::AscendingOrder);
	select();
	//removeColumn(0);
	setHeaderData(0, Qt::Horizontal, trUtf8("ID"));
	setHeaderData(1, Qt::Horizontal, trUtf8("Varenavn"));
	setHeaderData(2, Qt::Horizontal, trUtf8("Pris"));

	typeMap.insert(0, Types::Int);
	typeMap.insert(1, Types::String);
	typeMap.insert(2, Types::Currency);
}

void ItemsModel::enableEditing(bool enable)
{
	editingEnabled = enable;
}

Qt::ItemFlags ItemsModel::flags(const QModelIndex &index) const
{
	if(!index.isValid())
		return Qt::ItemIsEnabled;

	Qt::ItemFlags f = QSqlTableModel::flags(index);
	if(!editingEnabled)
		f &= (~Qt::ItemIsEditable);
	return f;

}

QVariant ItemsModel::data(const QModelIndex & item, int role) const
{
	QVariant data = QSqlTableModel::data(item, role);
	if(role == Types::TypeRole) {
		return QVariant(columnType(item.column()));
	} else if(role == Qt::DisplayRole) {
		return Types::formatData(data, columnType(item.column()));
	} else if(role == Qt::TextAlignmentRole && item.column() == 2) {
		return QVariant(Qt::AlignRight | Qt::AlignVCenter);
	}
	return data;
}

QString ItemsModel::getItemName(const QModelIndex & item) const
{
	return data(index(item.row(), 1), Qt::EditRole).toString();
}

double ItemsModel::getPrice(const QModelIndex & item) const
{
	return data(index(item.row(), 2), Qt::EditRole).toDouble();
}

int ItemsModel::columnType(int column) const
{
	return typeMap.value(column);
}

void ItemsModel::sort(int column, Qt::SortOrder order)
{
	submitAll();
	QSqlTableModel::sort(column, order);

}
