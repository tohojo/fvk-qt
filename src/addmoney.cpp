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
#include <QInputDialog>
#include <QMessageBox>

#include "addmoney.h"
#include "transactionsmodel.h"
#include "sqlhelpers.h"

AddMoney::AddMoney(QWidget * parent, TransactionsModel * t, int user)
	: QDialog(parent)
{
	transactions = t;
	userId = user;
	setupUi(this);
	setModal(true);
	amountField->setFocus(Qt::OtherFocusReason);
	connect(addButton, SIGNAL(clicked()), this, SLOT(accepted()));
}

void AddMoney::on_button50_clicked()
{
	amountField->setValue(50.0);
	amountField->setFocus(Qt::OtherFocusReason);
}

void AddMoney::on_button100_clicked()
{
	amountField->setValue(100.0);
	amountField->setFocus(Qt::OtherFocusReason);
}

void AddMoney::on_button200_clicked()
{
	amountField->setValue(200.0);
	amountField->setFocus(Qt::OtherFocusReason);
}

void AddMoney::accepted()
{
	qDebug() << "accepted()";
	amountField->interpretText();
	double amount = amountField->value();
	if(amount == 0.0) {
		return;
	}



	QString text = (amount > 0) ? trUtf8("Indsat") : trUtf8("Hævet");

	qDebug() << "Adding:" << userId << text << 1 << amount;

	transactions->addTransaction(userId, text, 1, amount, true);

	accept();
}
