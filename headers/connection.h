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
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QtDebug>
#include <QMessageBox>

static bool createConnection(QString database)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(database);
	if (!db.open()) {
		QMessageBox::critical(0, qApp->tr("Cannot open database"),
				qApp->tr("Kunne ikke forbinde til databasen.\n\n"
					"Klik Annuller for at afslutte."), QMessageBox::Cancel);
		return false;
	}

	qDebug() << "Connected to database:" << database;
	return true;
}

#endif
