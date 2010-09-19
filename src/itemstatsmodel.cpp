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

#include "itemstatsmodel.h"
#include "sqlhelpers.h"
#include "types.h"

ItemstatsModel::ItemstatsModel(QObject* parent)
	: TreeModel(parent)
{
}

ItemstatsModel::~ItemstatsModel()
{
}

void ItemstatsModel::setupModelData()
{
	QString q = appendTimeLimit(QString::fromUtf8("SELECT item_name, SUM(total_price) "
				"FROM transactions WHERE item_name <> 'Indsat' AND item_name <> 'Hævet'"));

	q.append("GROUP BY item_name ORDER BY item_name ASC");

	QSqlQuery query(q);
	while(query.next()) {
		QString name = query.value(0).toString();
		QString val = Types::formatData(query.value(1).toDouble(), Types::Currency).toString();
		QString qq = appendTimeLimit(
				QString::fromUtf8("SELECT c.name || ' ' || c.house, SUM(t.total_price) AS p "
				"FROM customers c, transactions t WHERE c.id = t.customer_id AND item_name = '%1' "
				).arg(name));
		qq.append(" GROUP BY t.customer_id ORDER BY p DESC LIMIT 3");
		addTop3(name, qq, "Ingen kunder fundet", val);

	}

}

