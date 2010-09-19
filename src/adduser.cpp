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
#include "adduser.h"
#include "usersmodel.h"

AddUser::AddUser(QWidget * parent, UsersModel * u)
	: QDialog(parent)
{
	setupUi(this);
	setModal(true);

	users = u;

	connect(createButton, SIGNAL(clicked()), this, SLOT(accepted()));
}

void AddUser::setCardNo(QString text)
{
	cardNoField->setText(text);
}

void AddUser::accepted()
{
	qDebug() << "accepted()";
	QString name = nameField->text();
	QString house = houseField->text();
	QString cardNo = cardNoField->text();

	if(name == "" || cardNo == "")
		return;

	users->addUser(name, house, cardNo);

	accept();
}
