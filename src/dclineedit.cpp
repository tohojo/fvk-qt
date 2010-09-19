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

#include <QWidget>
#include <QLineEdit>
#include <QFocusEvent>
#include <QMouseEvent>
#include <QtDebug>

#include "dclineedit.h"

DCLineEdit::DCLineEdit(QWidget * parent)
	: QLineEdit(parent)
{
	setReadOnly(true);
	editEnabled = false;
	connect(this, SIGNAL(returnPressed()), this, SLOT(on_returnPressed()));
}
DCLineEdit::DCLineEdit(const QString & contents, QWidget* parent)
	: QLineEdit(contents, parent)
{
}

void DCLineEdit::focusOutEvent(QFocusEvent * event)
{
//    qDebug() << "focusOutEvent received";
	event->ignore();
	setReadOnly(true);
}

void DCLineEdit::mouseDoubleClickEvent(QMouseEvent * event) 
{
//    qDebug() << "mouseDoubleClickEvent received";
	event->ignore();
	if(editEnabled)
		setReadOnly(false);
}

void DCLineEdit::on_returnPressed()
{
//    qDebug() << "returnPressed()";
	setReadOnly(true);
}

void DCLineEdit::setEditEnabled(bool en)
{
	editEnabled = en;
}
