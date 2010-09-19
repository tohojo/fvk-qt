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
#include <QDateTime>
#include <QSortFilterProxyModel>

#include "transactionsmodel.h"
#include "types.h"

TransactionsModel::TransactionsModel(QSortFilterProxyModel * selUsr)
	: QSqlTableModel()
{
	selectedUser = selUsr;

	setTable("transactions");
	setEditStrategy(QSqlTableModel::OnManualSubmit);
	setSort(2, Qt::AscendingOrder);
	select();
	//removeColumn(0);
	setHeaderData(0, Qt::Horizontal, trUtf8("ID"));
	setHeaderData(1, Qt::Horizontal, trUtf8("Kunde-ID"));
	setHeaderData(2, Qt::Horizontal, trUtf8("Tidspunkt"));
	setHeaderData(3, Qt::Horizontal, trUtf8("Varenavn"));
	setHeaderData(4, Qt::Horizontal, trUtf8("Antal"));
	setHeaderData(5, Qt::Horizontal, trUtf8("Pris"));

	typeMap.insert(0, Types::Int);
	typeMap.insert(1, Types::Int);
	typeMap.insert(2, Types::DateTime);
	typeMap.insert(3, Types::String);
	typeMap.insert(4, Types::Int);
	typeMap.insert(5, Types::Currency);
}

Qt::ItemFlags TransactionsModel::flags(const QModelIndex &index) const
{
	if(!index.isValid())
		return Qt::ItemIsEnabled;

	Qt::ItemFlags f = QSqlTableModel::flags(index);
	return f & (~Qt::ItemIsEditable);

}

QVariant TransactionsModel::data(const QModelIndex & item, int role) const
{
//    qDebug() << "Data:" << item << role;
	if(!item.isValid())
		return QVariant();

	QVariant data = QSqlTableModel::data(item, role);
	if(role == Types::TypeRole) {
		return QVariant(columnType(item.column()));
	}
	if(role == Qt::DisplayRole) {
//        qDebug() << "Columntype:" << columnType(item.column());
		return Types::formatData(data, columnType(item.column()));
	}
	if(role == Qt::TextAlignmentRole) {
		switch(item.column()) {
			case 4:
				return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
				break;
			case 5:
				return QVariant(Qt::AlignRight | Qt::AlignVCenter);
				break;
		}
	}
	return data;
}

int TransactionsModel::columnType(int column) const
{
	return typeMap.value(column);
}

// amount needs to be positive or negative with regards to the change
// to the balance. the absolute value will always be written to the db
void TransactionsModel::addTransaction(int user, QString text, 
		int count, double amount, bool updateTotalSpending, bool commit)
{
//    if(commit)
//        database().transaction();

	qDebug() << "addTransaction()" << "commit:" << commit;
	QSqlRecord record;
//    record.setValue(0, QVariant(5));
	if(text != QString::fromUtf8("Hævet"))
		amount = qAbs(amount);
	record.insert(0, QSqlField("id", QVariant::Int));
	record.insert(1, QSqlField("customer_id", QVariant::Int));
	record.insert(2, QSqlField("time", QVariant::UInt));
	record.insert(3, QSqlField("item_name", QVariant::String));
	record.insert(4, QSqlField("item_count", QVariant::Int));
	record.insert(5, QSqlField("total_price", QVariant::Double));
	record.setValue(1, QVariant(user));
	record.setValue(2, QVariant(QDateTime::currentDateTime().toTime_t()));
	record.setValue(3, QVariant(text));
	record.setValue(4, QVariant(count));
	record.setValue(5, QVariant(amount));

	if(!insertRecord(-1, record))
		qDebug() << lastError();

	if(commit) {
		double oldBalance = selectedUser->index(0, 3).data(Qt::EditRole).toDouble();
		double oldTotalSpending = 
			selectedUser->index(0, 4).data(Qt::EditRole).toDouble();

		selectedUser->setData(selectedUser->index(0,3), QVariant(oldBalance+amount));
		if(updateTotalSpending)
			selectedUser->setData(selectedUser->index(0,4), 
					QVariant(oldTotalSpending+amount));

		selectedUser->submit();
//        submitAll();
//        database().commit();
	}
}

void TransactionsModel::addMany(int user, QList<QString> & text, QList<int> & count,
	QList<double> & amount, bool updateTotalSpending)
{
	QTime t;
	t.start();
//    database().transaction();


	double totalAmount = 0;
	for(int i = 0; i < text.size(); i++) {
		addTransaction(user, text[i], count[i], amount[i], false, false);
		totalAmount += amount[i];
	}

	qDebug("Time elapsed: %d ms", t.elapsed());

	double oldBalance = selectedUser->index(0, 3).data(Qt::EditRole).toDouble();
	selectedUser->setData(selectedUser->index(0,3), QVariant(oldBalance+totalAmount));
	if(updateTotalSpending) {
		double oldTotalSpending = 
			selectedUser->index(0, 4).data(Qt::EditRole).toDouble();
		selectedUser->setData(selectedUser->index(0,4), 
				QVariant(oldTotalSpending+totalAmount));
	}
	qDebug("Time elapsed: %d ms", t.elapsed());

	selectedUser->submit();
	qDebug("Time elapsed: %d ms", t.elapsed());
//    submitAll();
	qDebug("Time elapsed: %d ms", t.elapsed());
//    if(!database().commit())
//        qDebug() << database().lastError();
	qDebug("Time elapsed: %d ms", t.elapsed());
}
