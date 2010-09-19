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
#ifndef SQLHELPERS_H
#define SQLHELPERS_H

#include <QtSql>

namespace SqlHelpers {
	static bool checkAdmin(QString cardNo)
	{
		QSqlQuery query(QString("SELECT COUNT(*) "
					"FROM customers WHERE card_id = '%1' AND admin = 1").arg(cardNo));
		if(query.next() && query.value(0).toInt() > 0)
			return true;

		return false;
	}

	static double getTotalBalance()
	{
		QSqlQuery query(QString("SELECT SUM(balance) "
					"FROM customers"));
		if(!query.next())
			return 0;
		return query.value(0).toDouble();
	}

	static double getTotalSpending()
	{
		QSqlQuery query(QString("SELECT SUM(total_spending) "
					"FROM customers"));
		if(!query.next())
			return 0;
		return query.value(0).toDouble();
	}
}

#endif
