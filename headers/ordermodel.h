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
#ifndef ORDERMODEL_H
#define ORDERMODEL_H

#include <QtCore>
#include "types.h"

class OrderModel: public QAbstractTableModel
{
	Q_OBJECT

public:
		OrderModel(QObject * parent =0);
		int rowCount(const QModelIndex & parent = QModelIndex()) const;
		int columnCount(const QModelIndex & parent = QModelIndex()) const;
		QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
		QVariant headerData(int section, Qt::Orientation orientation, 
				int role = Qt::DisplayRole) const;
		bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());
		void clear();
		void insertItem(QString name, int count, double price);
		bool removeItem(int row, int count);
		double getTotalPrice();
private:
		QList<QString> itemNames;
		QList<int> itemCounts;
		QList<double> itemTotalPrices;
		QList<double> itemPrices;
		QHash<int, int> typeMap;
		int rows;
};

#endif
