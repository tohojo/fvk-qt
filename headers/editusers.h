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
#ifndef EDITUSERS_H
#define EDITUSERS_H

#include <QApplication>
#include <QDialog>
#include "ui_editusers.h"
#include "usersmodel.h"
#include "transactionsmodel.h"

class EditUsers : public QDialog, private Ui::EditUsersForm
{
	Q_OBJECT

public:
		EditUsers(QWidget *parent = 0, UsersModel * us = 0, TransactionsModel * tr = 0);
private:
		UsersModel * users;
		TransactionsModel * transactions;
private slots:
		void on_zeroUserButton_clicked();
		void on_addUserButton_clicked();
		void on_deleteUserButton_clicked();
signals:
		void dataChanged();
};

#endif
