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
#ifndef TOTALMODEL_H
#define TOTALMODEL_H

#include <QObject>
#include <QDateTime>

#include "treemodel.h"
#include "treeitem.h"

class TotalModel : public TreeModel
{
	Q_OBJECT

	public:
		TotalModel(QObject* parent = 0);
		~TotalModel();
		void setupModelData();
	private:
		void addTotalBalance();
		void addTotalSpending();
		void addTopItems();
		void addTopCustomers();

};
#endif
