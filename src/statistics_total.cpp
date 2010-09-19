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

#include <QtGui>

#include "statistics_total.h"

StatisticsTotal::StatisticsTotal (QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);
	totalModel = new TotalModel(this);
	totalModel->setupModelData();
	totalView->setModel(totalModel);
	totalView->setColumnWidth(0, 250);

	itemModel = new ItemstatsModel(this);
	itemModel->setupModelData();
	itemView->setModel(itemModel);
	itemView->setColumnWidth(0, 250);
}

void StatisticsTotal::connectSignals(QCheckBox* since_start, QDateTimeEdit* start,
				QDateTimeEdit* end) 
{
	connectModel(since_start, start, end, totalModel);
	connectModel(since_start, start, end, itemModel);
}

void StatisticsTotal::connectModel(QCheckBox* since_start, QDateTimeEdit* start,
				QDateTimeEdit* end, TreeModel* model) 
{
	connect(since_start, SIGNAL(stateChanged(int)), model, SLOT(since_start_changed(int)));
	connect(start, SIGNAL(dateTimeChanged(QDateTime)), model, SLOT(start_time_changed(QDateTime)));
	connect(end, SIGNAL(dateTimeChanged(QDateTime)), model, SLOT(end_time_changed(QDateTime)));
}
