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
#ifndef TYPES_H
#define TYPES_H

#include <QDateTime>

namespace Types
{
	enum Type {
		Boolean,
		Int,
		Double,
		String,
		Currency,
		DateTime
	};

	const int TypeRole = 33;

	static QVariant formatData(QVariant data, int type) {
		switch(type) {
			case Boolean:
				return QVariant(data.toInt() ? 
						QObject::trUtf8("Ja") : QObject::trUtf8("Nej"));
				break;
			case Currency:
				return QVariant(
						QString("%1 kr").arg(data.toDouble(), 0, 'f', 2, QLatin1Char(' '))
						);
				break;
			case DateTime:
				{
//                    qDebug() << "DateTime type";
					QDateTime dt;
					dt.setTime_t(data.toUInt());
					return QVariant(dt.toString("yyyy-MM-dd hh:mm"));
					break;
				}
		}
		return data;
	}
}

#endif
