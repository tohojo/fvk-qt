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
#ifndef EDITITEMS_H
#define EDITITEMS_H

#include <QApplication>
#include <QDialog>
#include "ui_edititems.h"
#include "itemsmodel.h"

class EditItems : public QDialog, private Ui::EditItemsForm
{
	Q_OBJECT

public:
		EditItems(QWidget *parent = 0, ItemsModel * it = 0);

private:
		ItemsModel *items;

private slots:
		void on_addButton_clicked();
		void on_removeButton_clicked();

signals:
		void dataChanged();
};

#endif
