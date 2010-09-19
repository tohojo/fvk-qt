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
#include "edititems.h"
#include "itemsmodel.h"
#include "viewdelegate.h"

EditItems::EditItems(QWidget * parent, ItemsModel * it)
	: QDialog(parent)
{
	setupUi(this);
	setModal(true);
	items = it;

	itemsView->setModel(items);
	itemsView->setItemDelegate(new ViewDelegate);
	itemsView->sortByColumn(1, Qt::AscendingOrder);
    itemsView->setColumnHidden(0, true);
    itemsView->setColumnWidth(1, 200);
    itemsView->setSelectionMode(QAbstractItemView::SingleSelection);
    itemsView->setSelectionBehavior(QAbstractItemView::SelectRows);
	QHeaderView *v = itemsView->verticalHeader();
	v->setVisible(false);

	connect(closeButton, SIGNAL(clicked()), this, SLOT(accept()));


}

void EditItems::on_addButton_clicked()
{
	QItemSelectionModel * s = itemsView->selectionModel();
	int row = s->currentIndex().row();
	items->insertRows(row+1, 1);
	emit dataChanged();
}

void EditItems::on_removeButton_clicked()
{
	QItemSelectionModel * s = itemsView->selectionModel();
	int row = s->currentIndex().row();
	items->removeRows(row, 1);
	emit dataChanged();
}
