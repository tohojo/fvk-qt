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
#include <QtCore>
#include "ordermodel.h"
#include "types.h"

OrderModel::OrderModel(QObject * parent)
	: QAbstractTableModel(parent)
{
	rows = 0;
	typeMap.insert(0, Types::String);
	typeMap.insert(1, Types::Int);
	typeMap.insert(2, Types::Currency);
}

void OrderModel::clear()
{
	beginRemoveRows(QModelIndex(), 0, qMax(rows-1,0));
	itemNames.clear();
	itemCounts.clear();
	itemTotalPrices.clear();
	itemPrices.clear();
	rows = 0;
	endRemoveRows();
	reset();
}

void OrderModel::insertItem(QString name, int count, double price)
{
	int i = itemNames.indexOf(name);
	if(i > -1) {
		itemCounts.replace(i, itemCounts.value(i)+count);
		itemTotalPrices.replace(i, itemTotalPrices.value(i)+count*price);
		emit dataChanged(index(i, 0), index(i, 2));
	} else {
		beginInsertRows(QModelIndex(), rows, rows);
		itemNames.append(name);
		itemCounts.append(count);
		itemTotalPrices.append(count*price);
		itemPrices.append(price);
		rows++;
		endInsertRows();
	}
}

bool OrderModel::removeItem(int row, int count)
{
	if(count >= itemCounts.value(row))
		return removeRows(row, 1, QModelIndex());
	else {
		itemCounts.replace(row, itemCounts.value(row)-count);
		itemTotalPrices.replace(row, itemTotalPrices.value(row)-count*itemPrices.value(row));
		emit dataChanged(index(row, 0), index(row, 2));
	}
	return true;
}

bool OrderModel::removeRows(int row, int count, const QModelIndex & parent)
{
	if(row + count > rows)
		return false;

	beginRemoveRows(parent, row, row+count-1);
	for(int i = row; i < row+count; i++) {
		itemNames.removeAt(i);
		itemCounts.removeAt(i);
		itemTotalPrices.removeAt(i);
		itemPrices.removeAt(i);
		rows--;
	}
	endRemoveRows();

	return true;
}

int OrderModel::rowCount(const QModelIndex & parent) const
{
	return rows;
}

int OrderModel::columnCount(const QModelIndex & parent) const
{
	return 3;
}

QVariant OrderModel::data(const QModelIndex & index, int role) const
{
	if(index.isValid()) {
		if(role == Qt::TextAlignmentRole) {
			switch(index.column()) {
				case 0:
					return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
					break;
				case 1:
					return QVariant(Qt::AlignHCenter | Qt::AlignVCenter);
					break;
				case 2:
					return QVariant(Qt::AlignRight | Qt::AlignVCenter);
					break;
			}
		} else if(role == Qt::DisplayRole) {
			switch(index.column()) {
				case 0:
					return Types::formatData(
							QVariant(itemNames[index.row()]), 
							typeMap.value(0));
					break;
				case 1:
					return Types::formatData(
							QVariant(itemCounts[index.row()]),
							typeMap.value(1));
					break;
				case 2:
					return Types::formatData(
							QVariant(itemTotalPrices[index.row()]),
							typeMap.value(2));
					break;
			}
		} else if(role == Qt::EditRole) {
			switch(index.column()) {
				case 0:
					return QVariant(itemNames[index.row()]);
					break;
				case 1:
					return QVariant(itemCounts[index.row()]);
					break;
				case 2:
					return QVariant(itemTotalPrices[index.row()]);
					break;
			}
		}
	}
	return QVariant();
}


QVariant OrderModel::headerData(int section, Qt::Orientation orientation, 
				int role) const
{
	if(role == Qt::DisplayRole) {
		if(orientation == Qt::Vertical)
			return QVariant(section);
		else {
			switch(section) {
				case 0:
					return QVariant(trUtf8("Varenavn"));
					break;
				case 1:
					return QVariant(trUtf8("Antal"));
					break;
				case 2:
					return QVariant(trUtf8("Total pris"));
					break;
			}
		}
	}
	return QVariant();
}

double OrderModel::getTotalPrice()
{
	double price = 0;
	foreach(double p, itemTotalPrices)
		price += p;
	return price;
}
