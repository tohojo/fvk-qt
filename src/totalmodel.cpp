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

#include <QDebug>

#include "totalmodel.h"
#include "sqlhelpers.h"
#include "types.h"

TotalModel::TotalModel(QObject* parent)
	: TreeModel(parent)
{
}

TotalModel::~TotalModel()
{
}

void TotalModel::setupModelData()
{
	addTotalBalance();
	addTotalSpending();
	addTopItems();
	addTopCustomers();
}

void TotalModel::addTotalBalance()
{
	addItem("Total balance (nu)", SqlHelpers::getTotalBalance());

	QString qq = QString::fromUtf8("SELECT name || ' ' || house AS name, balance "
			"FROM customers ORDER BY balance DESC LIMIT 3");
	addTop3("Top 3 balance (nu)", qq, "Ingen brugere fundet", "");
}


void TotalModel::addTotalSpending()
{
	double deposit = getDoubleValue(QString::fromUtf8("SELECT SUM(total_price) FROM transactions "
			"WHERE (item_name = 'Indsat' OR item_name = 'Hævet')"));
	double spending = getDoubleValue(QString::fromUtf8("SELECT SUM(total_price) FROM transactions "
			"WHERE item_name <> 'Indsat' AND item_name <> 'Hævet'"));

	addItem(trUtf8("Totalt indsat (periode)"), deposit);
	addItem(trUtf8("Totalt brugt (periode)"), spending);
}

void TotalModel::addTopItems()
{
	QString q = appendTimeLimit(
			QString::fromUtf8("SELECT item_name, SUM(total_price) AS p FROM transactions "
			"WHERE item_name <> 'Indsat' AND item_name <> 'Hævet'"));
	q.append("GROUP BY item_name ORDER BY p DESC LIMIT 3");

	addTop3("Top 3 varer", q, "Ingen varer fundet", "");
}

void TotalModel::addTopCustomers()
{
	QString q = appendTimeLimit(
			QString::fromUtf8("SELECT c.name || ' ' || c.house AS name, SUM(t.total_price) AS p "
			"FROM transactions t, customers c "
			"WHERE c.id = t.customer_id AND t.item_name <> 'Indsat' AND t.item_name <> 'Hævet'"));
	q.append("GROUP BY t.customer_id ORDER BY p DESC LIMIT 3");


	addTop3("Top 3 brugere", q, "Ingen brugere fundet", "");

}
