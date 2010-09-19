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
#ifndef ITEMSMODEL_H
#define ITEMSMODEL_H

#include <QtSql>

class ItemsModel : public QSqlTableModel
{
	public:
		ItemsModel();
		void enableEditing(bool enable);
		Qt::ItemFlags flags(const QModelIndex &index) const;
		QVariant data(const QModelIndex & item, int role = Qt::DisplayRole) const;
		QString getItemName(const QModelIndex & item) const;
		double getPrice(const QModelIndex & item) const;
		int columnType(int column) const;
		void sort(int column, Qt::SortOrder order);
	private:
		bool editingEnabled;
		QHash<int, int> typeMap;

};

#endif
