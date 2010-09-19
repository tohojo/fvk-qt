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

#ifndef DCLINEEDIT_H
#define DCLINEEDIT_H

#include <QWidget>
#include <QLineEdit>
#include <QFocusEvent>
#include <QMouseEvent>

class DCLineEdit : public QLineEdit
{
	Q_OBJECT
	public:
		DCLineEdit(QWidget* parent = 0);
		DCLineEdit(const QString & contents, QWidget* parent = 0);
		void focusOutEvent(QFocusEvent * event);
		void mouseDoubleClickEvent(QMouseEvent * event);
		void setEditEnabled(bool en);
	private:
		bool editEnabled;
	private slots:
		void on_returnPressed();
};

#endif
