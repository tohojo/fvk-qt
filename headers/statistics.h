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
#ifndef STATISTICS_H
#define STATISTICS_H

#include <QApplication>
#include <QDialog>
#include "ui_statistics.h"
#include "statistics_total.h"

class Statistics : public QDialog, private Ui::StatisticsForm
{
	Q_OBJECT

public:
		Statistics(QWidget *parent = 0);
private:
		StatisticsTotal total;
private slots:
		void since_start_changed(int);
};
#endif

