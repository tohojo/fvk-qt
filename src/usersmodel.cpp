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
#include "usersmodel.h"
#include "types.h"

UsersModel::UsersModel()
	: QSqlTableModel()
{
	setTable("customers");
	setEditStrategy(QSqlTableModel::OnManualSubmit);
	setSort(0, Qt::AscendingOrder);
	select();
	//removeColumn(0);
	setHeaderData(0, Qt::Horizontal, trUtf8("ID"));
	setHeaderData(1, Qt::Horizontal, trUtf8("Navn"));
	setHeaderData(2, Qt::Horizontal, trUtf8("Hus"));
	setHeaderData(3, Qt::Horizontal, trUtf8("Saldo"));
	setHeaderData(4, Qt::Horizontal, trUtf8("Totalt forbrug"));
	setHeaderData(5, Qt::Horizontal, trUtf8("Kort-ID"));
	setHeaderData(6, Qt::Horizontal, trUtf8("Kredit"));
	setHeaderData(7, Qt::Horizontal, trUtf8("Admin"));

	typeMap.insert(0, Types::Int);
	typeMap.insert(1, Types::String);
	typeMap.insert(2, Types::String);
	typeMap.insert(3, Types::Currency);
	typeMap.insert(4, Types::Currency);
	typeMap.insert(5, Types::String);
	typeMap.insert(6, Types::Boolean);
	typeMap.insert(7, Types::Boolean);
}

Qt::ItemFlags UsersModel::flags(const QModelIndex &index) const
{
	if(!index.isValid())
		return Qt::ItemIsEnabled;

	Qt::ItemFlags f = QSqlTableModel::flags(index);
	if(index.column() == 0 || index.column() == 3 || index.column() == 4)
		f &= (~Qt::ItemIsEditable);
	return f;

}

QVariant UsersModel::data(const QModelIndex & item, int role) const
{
//    qDebug() << "Data:" << item << role;
	if(!item.isValid())
		return QVariant();

	QVariant data = QSqlTableModel::data(item, role);
	if(role == Types::TypeRole) {
		return QVariant(columnType(item.column()));
	}
	if(role == Qt::DisplayRole) {
		return Types::formatData(data, columnType(item.column()));
	}
	if(role == Qt::ForegroundRole) {
		if(item.column() == 3 || item.column() == 4) {
			if(item.data(Qt::EditRole).toDouble() < 0) {
				return QBrush(Qt::red);
			}
		}
	}
	if(role == Qt::TextAlignmentRole) {
		if(item.column() == 3 || item.column() == 4) {
			return QVariant(Qt::AlignRight | Qt::AlignVCenter);
		}

	}
	return data;
}

int UsersModel::columnType(int column) const
{
	return typeMap.value(column);
}

void UsersModel::addUser(QString name, QString house, QString cardNo)
{
	database().transaction();

	qDebug() << "addUser()";
	QSqlRecord record;
//    record.setValue(0, QVariant(5));
	record.insert(0, QSqlField("id", QVariant::Int));
	record.insert(1, QSqlField("name", QVariant::String));
	record.insert(2, QSqlField("house", QVariant::String));
	record.insert(3, QSqlField("balance", QVariant::Double));
	record.insert(4, QSqlField("total_spending", QVariant::Double));
	record.insert(5, QSqlField("card_id", QVariant::String));
	record.insert(6, QSqlField("credit", QVariant::Int));
	record.insert(7, QSqlField("admin", QVariant::Int));
	record.setValue(1, QVariant(name));
	record.setValue(2, QVariant(house));
	record.setValue(3, QVariant(0));
	record.setValue(4, QVariant(0));
	record.setValue(5, QVariant(cardNo));
	record.setValue(6, QVariant(0));
	record.setValue(7, QVariant(0));

	if(!insertRecord(-1, record))
		qDebug() << lastError();

	submitAll();
	database().commit();
}

void UsersModel::sort(int column, Qt::SortOrder order)
{
	submitAll();
	QSqlTableModel::sort(column, order);

}
