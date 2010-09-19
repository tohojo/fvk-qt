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
#ifndef ADDMONEY_H
#define ADDMONEY_H

#include <QApplication>
#include <QDialog>
#include "ui_addmoney.h"
#include "transactionsmodel.h"

class AddMoney : public QDialog, private Ui::AddMoneyForm
{
	Q_OBJECT

public:
		AddMoney(QWidget *parent = 0, TransactionsModel * t = 0, int user = 0);
private:
		TransactionsModel * transactions;
		int userId;
private slots:
		void on_button50_clicked();
		void on_button100_clicked();
		void on_button200_clicked();
		void accepted();
};
#endif
