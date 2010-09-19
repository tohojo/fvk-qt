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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QSortFilterProxyModel>
#include "ui_mainwindow.h"
#include "readcarddialog.h"
#include "addmoney.h"
#include "adduser.h"
#include "edititems.h"
#include "editusers.h"
#include "itemsmodel.h"
#include "ordermodel.h"
#include "usersmodel.h"
#include "transactionsmodel.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT

public:
		MainWindow(QWidget *parent = 0);
		~MainWindow();
private:
		ItemsModel *items;
		OrderModel *order;
		UsersModel *users;
		TransactionsModel *transactions;
		QSortFilterProxyModel * selectedUser;
		QSortFilterProxyModel * userTransactions;
		ReadCardDialog * readCard;
		QLineEdit * readCardField;
		int userLoaded;
		bool checkAdmin(QString what);

private slots:
	void on_addMoneyButton_clicked();
	void on_actionLoadUser_triggered();
	void on_actionEditItems_triggered();
	void on_actionAddUser_triggered();
	void on_actionEditUsers_triggered();
	void on_actionAbout_triggered();
	void on_actionStatistics_triggered();
	void on_addButton_clicked();
	void on_removeButton_clicked();
	void on_doOrderButton_clicked();
	void updateUser();
	void updateTotalPrice();
	void unloadUser();
	void selectNewOrderItem();
	void selectUpdatedOrderItem(QModelIndex topLeft);
	void saveDb();
	void finishedLoading();
	void addItem(QModelIndex i);
	void removeItem(QModelIndex i);

signals:
	void dataChanged();
};

#endif
