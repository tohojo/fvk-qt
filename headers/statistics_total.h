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
#ifndef STATISTICS_TOTAL_H
#define STATISTICS_TOTAL_H

#include <QApplication>
#include <QWidget>
#include <QDateTime>
#include <QtGui>

#include "ui_statistics_total.h"
#include "totalmodel.h"
#include "itemstatsmodel.h"

class StatisticsTotal : public QWidget, private Ui::StatisticsTotal
{
	Q_OBJECT

public:
		StatisticsTotal(QWidget *parent = 0);
		void connectSignals(QCheckBox* since_start, QDateTimeEdit* start,
				QDateTimeEdit* end);
private:
		void connectModel(QCheckBox* since_start, QDateTimeEdit* start,
				QDateTimeEdit* end, TreeModel* model);
		TotalModel* totalModel;
		ItemstatsModel* itemModel;
};
#endif

