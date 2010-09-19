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
#ifndef USERSMODEL_H
#define USERSMODEL_H

#include <QtSql>

class UsersModel : public QSqlTableModel
{
	Q_OBJECT

	public:
		UsersModel();
		QVariant data(const QModelIndex & item, int role = Qt::DisplayRole) const;
		Qt::ItemFlags flags(const QModelIndex &index) const;
		int columnType(int column) const;
		void addUser(QString name, QString house, QString cardNo);
		void sort(int column, Qt::SortOrder order);
	private:
		QHash<int, int> typeMap;
};

#endif
