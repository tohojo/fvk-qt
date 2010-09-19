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
#include <QtDebug>
#include <QHeaderView>
#include <QMessageBox>
#include <QSortFilterProxyModel>

#include "constants.h"
#include "mainwindow.h"
#include "addmoney.h"
#include "adduser.h"
#include "edititems.h"
#include "editusers.h"
#include "itemsmodel.h"
#include "ordermodel.h"
#include "usersmodel.h"
#include "statistics.h"
#include "transactionsmodel.h"
#include "viewdelegate.h"
#include "sqlhelpers.h"
#include "ui_readcard.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
    setupUi(this);

    setWindowTitle(trUtf8("NatCafé FVK-system v%1").arg(FVK_VERSION));
    versionLabel->setText(trUtf8("Version %1").arg(FVK_VERSION));

	connect(this, SIGNAL(dataChanged()), this, SLOT(saveDb()));

	readCard = new ReadCardDialog(this);
	readCardField = new QLineEdit(this);
	readCardField->setVisible(false);

	connect(readCardField, SIGNAL(returnPressed()), this, SLOT(finishedLoading()));


	// setup items
	items = new ItemsModel;
	itemsView->setModel(items);
	itemsView->sortByColumn(1, Qt::AscendingOrder);
    itemsView->setColumnHidden(0, true);
	itemsView->setColumnWidth(1, 150);
    itemsView->setSelectionMode(QAbstractItemView::SingleSelection);
    itemsView->setSelectionBehavior(QAbstractItemView::SelectRows);
	itemsView->verticalHeader()->setVisible(false);
	connect(itemsView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(addItem(QModelIndex)));

	// setup order
	order = new OrderModel;
	connect(order, SIGNAL(dataChanged(QModelIndex, QModelIndex)), 
			this, SLOT(selectUpdatedOrderItem(QModelIndex)));
	connect(order, SIGNAL(rowsInserted(QModelIndex, int, int)), 
			this, SLOT(selectNewOrderItem()));
	orderView->setModel(order);
	orderView->setColumnWidth(0, 140);
	orderView->setColumnWidth(1, 50);
	orderView->verticalHeader()->setVisible(false);
	connect(orderView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(removeItem(QModelIndex)));

	// setup users
	users = new UsersModel;
	selectedUser = new QSortFilterProxyModel;
	selectedUser->setSourceModel(users);
	selectedUser->setFilterKeyColumn(5); // kort-ID
	selectedUser->setDynamicSortFilter(true);
	connect(selectedUser, SIGNAL(dataChanged(QModelIndex, QModelIndex)), 
			this, SLOT(updateUser()));
	userLoaded = 0;

	// setup transactions
	transactions = new TransactionsModel(selectedUser);
	userTransactions = new QSortFilterProxyModel;
	userTransactions->setSourceModel(transactions);
	userTransactions->setFilterKeyColumn(1); // kunde-id
	transactionView->setModel(userTransactions);
	// hide ID and customer id
	transactionView->setColumnHidden(0, true);
	transactionView->setColumnHidden(1, true);
	// set widths (values from experimenting)
	transactionView->setColumnWidth(2, 125);
	transactionView->setColumnWidth(3, 115);
	transactionView->setColumnWidth(4, 40);
	transactionView->setColumnWidth(5, 85);
	// hide the vertical header
	transactionView->verticalHeader()->setVisible(false);
	connect(userTransactions, SIGNAL(dataChanged(QModelIndex, QModelIndex)), 
			transactionView, SLOT(scrollToBottom()));
	connect(userTransactions, SIGNAL(layoutChanged()), 
			transactionView, SLOT(scrollToBottom()));
	// filter on a non-existant user to clear the list
	unloadUser();


	updateTotalPrice();
}

MainWindow::~MainWindow()
{
	saveDb();

	delete items;
	delete readCard;
	delete order;
	delete users;
	delete selectedUser;
	delete transactions;
	delete userTransactions;
}

void MainWindow::on_addMoneyButton_clicked()
{

	if(userLoaded) {
		if(!checkAdmin(trUtf8("indsætte penge")))
			return;

		AddMoney addMoney(this, transactions, userLoaded);
		addMoney.exec();
		transactionView->scrollToBottom();
		emit dataChanged();
	} else
		QMessageBox::critical(this, trUtf8("Indsæt penge"),
				trUtf8("Der skal være indlæst en bruger for at indsætte penge"),
				QMessageBox::Ok);
}

void MainWindow::on_actionAddUser_triggered()
{
	AddUser addUser(this, users);
	addUser.exec();
	emit dataChanged();
}

void MainWindow::on_actionEditItems_triggered()
{
	if(!checkAdmin(trUtf8("rette varer")))
		return;

	items->enableEditing(true);
	EditItems editItems(this,items);
	connect(&editItems, SIGNAL(dataChanged()), this, SLOT(saveDb()));
	editItems.exec();
	emit dataChanged();
	items->enableEditing(false);
	order->clear();
	updateTotalPrice();
}

void MainWindow::on_actionEditUsers_triggered()
{
	if(!checkAdmin(trUtf8("rette brugere")))
		return;

	EditUsers editUsers(this, users, transactions);
	connect(&editUsers, SIGNAL(dataChanged()), this, SLOT(saveDb()));
	editUsers.exec();
	emit dataChanged();
	updateTotalPrice();
}

void MainWindow::on_actionLoadUser_triggered()
{
	readCardField->grabKeyboard();
}

void MainWindow::finishedLoading()
{
	readCardField->releaseKeyboard();
	QString text = readCardField->text();
	readCardField->setText("");

	qDebug() << "Got card no:" << text;

	QString oldFilter = selectedUser->filterRegExp().pattern();
	selectedUser->setFilterRegExp(QString("^%1$").arg(text));
	if(selectedUser->rowCount() > 0)
		userLoaded = selectedUser->index(0,0).data().toInt();
	else {
		QMessageBox::critical(this, trUtf8("Bruger ikke fundet"),
				trUtf8("Den indlæste bruger blev ikke fundet"),
				QMessageBox::Ok);
		AddUser addUser(this, users);
		addUser.setCardNo(text);
		addUser.exec();
		emit dataChanged();
		if(selectedUser->rowCount() > 0)
			userLoaded = selectedUser->index(0,0).data().toInt();
		else
			selectedUser->setFilterRegExp(oldFilter);
	}
	updateUser();
	updateTotalPrice();

}

void MainWindow::on_addButton_clicked()
{
	QItemSelectionModel * s = itemsView->selectionModel();
	QModelIndex i = s->currentIndex();
	addItem(i);
}

void MainWindow::addItem(QModelIndex i)
{
	if(!i.isValid())
		return;
	QModelIndex name = i.sibling(i.row(), 1);
	QModelIndex price = i.sibling(i.row(), 2);
	int count = countField->value();
	order->insertItem(
			name.data(Qt::EditRole).toString(),
			count,
			price.data(Qt::EditRole).toDouble()
			);
	countField->setValue(1);
	updateTotalPrice();
}

void MainWindow::on_removeButton_clicked()
{
	QItemSelectionModel * s = orderView->selectionModel();
	QModelIndex i = s->currentIndex();
	removeItem(i);
}

void MainWindow::removeItem(QModelIndex i)
{
	if(!i.isValid())
		return;
	int count = countField->value();
	order->removeItem(i.row(), count);
	countField->setValue(1);
	updateTotalPrice();
}

void MainWindow::selectUpdatedOrderItem(QModelIndex topLeft)
{
	QItemSelectionModel * s = orderView->selectionModel();
	s->setCurrentIndex(topLeft, QItemSelectionModel::SelectCurrent | 
			QItemSelectionModel::Rows |
			QItemSelectionModel::Clear);
}

void MainWindow::selectNewOrderItem()
{
	QModelIndex i = order->index(order->rowCount()-1, 0);
	QItemSelectionModel * s = orderView->selectionModel();
	s->setCurrentIndex(i, QItemSelectionModel::SelectCurrent | 
			QItemSelectionModel::Rows |
			QItemSelectionModel::Clear);

}

void MainWindow::on_actionAbout_triggered()
{
	QString text = 
trUtf8("<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal; text-decoration:none;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">NatCafé FVK-system v%1</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Kontokortsystem til fredagsbaren på NAT, RUC</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Copyright &copy; 2007 Toke Høiland-Jørgensen, toke@toke.dk</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left"
        ":0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">This program is free software: you can redistribute it and/or modify</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">it under the terms of the GNU General Public License as published </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">by the Free Software Foundation, either version 3 of the License, </p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">or (at your option) any later version.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-botto"
        "m:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">This program is distributed in the hope that it will be useful,</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">but WITHOUT ANY WARRANTY; without even the implied warranty of</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">GNU General Public License for more details.</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"></p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">You should have received a copy of the "
        "GNU General Public License</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">along with this program.  If not, see <a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>.</p></body></html>").arg(FVK_VERSION);
	QMessageBox::information(this, trUtf8("Om NatCafé FVK-system"),
			text,
			QMessageBox::Ok);

}

void MainWindow::updateTotalPrice()
{
	qDebug() << "updateTotalPrice()";
	totalPriceLabel->setText(
			trUtf8("Total pris: %1 kr").arg(order->getTotalPrice(), 0, 
				'f', 2, QLatin1Char(' '))
			);
	QPalette palette;
	QColor color = Qt::black;

	if(userLoaded)
	{
		double balance = selectedUser->index(0,3).data(Qt::EditRole).toDouble();
		bool credit = selectedUser->index(0,6).data(Qt::EditRole).toBool();
		if(!credit && balance < order->getTotalPrice()) {
			color = Qt::red;
		}
	}

	palette.setColor(totalPriceLabel->foregroundRole(), color);
	totalPriceLabel->setPalette(palette);
}

void MainWindow::updateUser()
{
	QPalette palette;
	QColor color = Qt::black;

	if(userLoaded) {
		nameField->setText(selectedUser->index(0, 1).data().toString());
		houseField->setText(selectedUser->index(0, 2).data().toString());
		cardnoField->setText(selectedUser->index(0, 5).data().toString());
		creditLabel->setText(selectedUser->index(0, 6).data().toString());
		totalSpendingLabel->setText(selectedUser->index(0, 4).data().toString());
		double balance = selectedUser->index(0,3).data(Qt::EditRole).toDouble();
		if(balance < 0)
			color = Qt::red;
		balanceLabel->setText(selectedUser->index(0, 3).data().toString());
		userTransactions->setFilterRegExp(QString("^%1$").arg(
				selectedUser->index(0, 0).data().toString()));
		transactionView->scrollToBottom();
	} else {
		nameField->setText(trUtf8("Ingen bruger indlæst"));
		houseField->setText("");
		cardnoField->setText("");
		creditLabel->setText(trUtf8("Nej"));
		totalSpendingLabel->setText(trUtf8("0.00 kr"));
		balanceLabel->setText(trUtf8("0.00 kr"));
		userTransactions->setFilterFixedString("-1");
	}

	palette.setColor(balanceLabel->foregroundRole(), color);
	balanceLabel->setPalette(palette);
}

void MainWindow::on_doOrderButton_clicked()
{
	qDebug() << "on_doOrderButton_clicked()";
	if(!userLoaded)
		return;

	double balance = selectedUser->index(0,3).data(Qt::EditRole).toDouble();
	bool credit = selectedUser->index(0,6).data(Qt::EditRole).toBool();

	qDebug() << "Balance/credit/totalPrice:" << balance << credit << order->getTotalPrice();

	if(order->getTotalPrice() > balance && !credit) {
		QMessageBox::critical(this, trUtf8("Ikke penge nok"),
				trUtf8("Der er ikke penge nok på brugerens konto,\nog brugeren har ikke kredit."),
				QMessageBox::Ok);
		return;
	}

	QList<QString> names;
	QList<int> counts;
	QList<double> amounts;

	for(int i = 0; i < order->rowCount(); i++) {
		names.append(order->index(i, 0).data(Qt::EditRole).toString());
		counts.append(order->index(i, 1).data(Qt::EditRole).toInt());
		amounts.append(-order->index(i, 2).data(Qt::EditRole).toDouble());

	}
	transactions->addMany(userLoaded, names, counts, amounts);


	order->clear();
	updateTotalPrice();
	transactionView->scrollToBottom();


	balance = selectedUser->index(0,3).data(Qt::EditRole).toDouble();
	QMessageBox::information(this, trUtf8("Saldo"),
			trUtf8("Brugeren har nu %1 kr tilbage på kontoen").arg(balance),
			QMessageBox::Ok);

	unloadUser();

	emit dataChanged();
}

void MainWindow::unloadUser()
{

	userTransactions->setFilterFixedString("-1");
	userLoaded = 0;
	updateUser();
}


void MainWindow::saveDb()
{
	qDebug() << "saveDb()";
	QSqlDatabase db = QSqlDatabase::database();
	db.transaction();

	// submit each database model and make sure all rows are loaded from the database
	// so that the transaction can be committed successfully

	transactions->submitAll();
	while (transactions->canFetchMore())
		transactions->fetchMore();

	users->submitAll();
	while (users->canFetchMore())
		users->fetchMore();

	items->submitAll();
	while (items->canFetchMore())
		items->fetchMore();
	if(!db.commit())
		qDebug() << db.lastError();

	transactionView->scrollToBottom();
}

bool MainWindow::checkAdmin(QString what)
{
	bool ok;
	QString response = QInputDialog::getText(this, trUtf8("Indlæs admin"),
			trUtf8("Indlæs et kort tilhørende en admin for at %1").arg(what), 
			QLineEdit::Normal, "", &ok); 

	if(!ok) 
		return false;

	if(!SqlHelpers::checkAdmin(response)) {
		QMessageBox::critical(this, trUtf8("Indlæs admin"),
				trUtf8("Det indlæste kort tilhører ikke en admin"),
				QMessageBox::Ok);
		return false;
	}

	return true;
}

void MainWindow::on_actionStatistics_triggered()
{
	/*QMessageBox::information(this, trUtf8("Statistik"),
			trUtf8("Total balance: %1 kr\nTotalt forbrug: %2 kr").arg(
				SqlHelpers::getTotalBalance()).arg(
				SqlHelpers::getTotalSpending()),
			QMessageBox::Ok);*/
	Statistics stats;
	stats.exec();
}
