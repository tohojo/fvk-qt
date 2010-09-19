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

#include "statistics.h"

Statistics::Statistics (QWidget *parent)
	: QDialog(parent)
{
	setupUi(this);
	connect(sinceStart, SIGNAL(stateChanged(int)), this, SLOT(since_start_changed(int)));

	tabWidget->clear();
	tabWidget->addTab(&total, "Total");
	total.connectSignals(sinceStart, startTime, endTime);

	QDateTime cur = QDateTime::currentDateTime();
	startTime->setDateTime(cur.addDays(-1));
	endTime->setDateTime(cur);

}

void Statistics::since_start_changed(int state)
{
	if(state == Qt::Checked) {
		startTime->setDisabled(true);
		endTime->setDisabled(true);
	} else {
		startTime->setEnabled(true);
		endTime->setEnabled(true);
	}
}
