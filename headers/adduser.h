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
#include <QApplication>
#include <QDialog>
#include "ui_adduser.h"
#include "usersmodel.h"

#ifndef ADDUSER_H
#define ADDUSER_H

class AddUser : public QDialog, private Ui::AddUserForm
{
	Q_OBJECT

public:
		AddUser(QWidget *parent = 0, UsersModel * u = 0);
		void setCardNo(QString text);

private:
		UsersModel * users;

private slots:
		void accepted();
};

#endif
