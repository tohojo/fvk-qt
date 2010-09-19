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
#ifndef TRANSACTIONSMODEL_H
#define TRANSACTIONSMODEL_H

#include <QtSql>
#include <QSortFilterProxyModel>

class TransactionsModel : public QSqlTableModel
{
	Q_OBJECT

	public:
		TransactionsModel(QSortFilterProxyModel * selUsr);
		QVariant data(const QModelIndex & item, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		int columnType(int column) const;
		void addTransaction(int user, QString text, int count, double amount, 
				bool updateTotalSpending = false, bool commit = true);
		void addMany(int user, QList<QString> & text, QList<int> & count, 
				QList<double> & amount, bool updateTotalSpending = false);
	private:
		QSortFilterProxyModel * selectedUser;
		QHash<int, int> typeMap;
};

#endif
