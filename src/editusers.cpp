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
#include <QHeaderView>
#include <QMessageBox>
#include "editusers.h"
#include "usersmodel.h"
#include "viewdelegate.h"
#include "transactionsmodel.h"

EditUsers::EditUsers(QWidget * parent, UsersModel * us, TransactionsModel * tr)
	: QDialog(parent)
{
	setupUi(this);
	setModal(true);
	users = us;
	transactions = tr;

	usersView->setModel(users);

	usersView->setItemDelegate(new ViewDelegate);

	usersView->sortByColumn(1, Qt::AscendingOrder);
	usersView->setColumnHidden(0, true);
	usersView->setColumnWidth(1, 180);
	usersView->setColumnWidth(2, 50);
	usersView->setColumnWidth(3, 80);
	usersView->setColumnWidth(4, 95);
	usersView->setColumnWidth(5, 100);
	usersView->setColumnWidth(6, 50);
	usersView->setColumnWidth(7, 50);
	usersView->setSelectionMode(QAbstractItemView::SingleSelection);
	usersView->setSelectionBehavior(QAbstractItemView::SelectRows);
	QHeaderView *v = usersView->verticalHeader();
	v->setVisible(false);

	connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));
}

void EditUsers::on_zeroUserButton_clicked()
{
	QItemSelectionModel * s = usersView->selectionModel();
	QModelIndex i = s->currentIndex();
	if(!i.isValid())
		return;

	double balance = i.sibling(i.row(),3).data(Qt::EditRole).toDouble();
	int user = i.sibling(i.row(), 0).data(Qt::EditRole).toInt();
	double amount = -balance;
	QString text = (amount > 0) ? trUtf8("Indsat") : trUtf8("Hævet");
	transactions->addTransaction(user, text, 1, amount, true);

}

void EditUsers::on_deleteUserButton_clicked()
{
	QItemSelectionModel * s = usersView->selectionModel();
	QModelIndex i = s->currentIndex();
	if(!i.isValid())
		return;

	int ret = QMessageBox::information(this, trUtf8("FVK"),
			trUtf8("Er du sikker på at du vil slette denne bruger?"),
			QMessageBox::Yes | QMessageBox::No);

	if(ret == QMessageBox::Yes) {
		int row = s->currentIndex().row();
		users->removeRows(row, 1);
		emit dataChanged();
	}
}

void EditUsers::on_addUserButton_clicked()
{
	QItemSelectionModel * s = usersView->selectionModel();
	int row = s->currentIndex().row();
	users->insertRows(row+1, 1);

//    qDebug() << usersView->columnWidth(1) << usersView->columnWidth(2)
//        << usersView->columnWidth(3) << usersView->columnWidth(4)
//        << usersView->columnWidth(5) << usersView->columnWidth(6)
//        << usersView->columnWidth(7);
}
