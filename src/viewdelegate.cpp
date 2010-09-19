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
#include <QtCore>
#include <QtGui>
#include "viewdelegate.h"
#include "types.h"

ViewDelegate::ViewDelegate(QObject * parent)
	: QItemDelegate(parent)
{
}

QWidget *ViewDelegate::createEditor(QWidget *parent,
		const QStyleOptionViewItem &/* option */,
		const QModelIndex & index ) const
{
	int columnType = index.data(Types::TypeRole).toInt();
	switch(columnType) {
		case Types::Boolean:
			{
				QComboBox *editor = new QComboBox(parent);
				editor->addItem(trUtf8("Nej"));
				editor->addItem(trUtf8("Ja"));
				return editor;
				break;
			}

		case Types::Int:
			{
				QSpinBox *editor = new QSpinBox(parent);
				editor->setMinimum(-100000);
				editor->setMaximum(100000);
				return editor;
				break;
			}

		case Types::Double:
		case Types::Currency:
			{
				QDoubleSpinBox *editor = new QDoubleSpinBox(parent);
				editor->setMinimum(-100000);
				editor->setMaximum(100000);
				if(columnType == Types::Currency)
					editor->setSuffix(trUtf8(" kr"));

				return editor;
				break;
			}

		default:
			{
				QLineEdit *editor = new QLineEdit(parent);
				return editor;
				break;
			}

	}
}

void ViewDelegate::setEditorData(QWidget *editor,
		const QModelIndex &index) const
{
	int columnType = index.data(Types::TypeRole).toInt();
	switch(columnType) {
		case Types::Boolean:
			{
				int value = index.model()->data(index, Qt::EditRole).toInt();
				QComboBox *cBox = static_cast<QComboBox*>(editor);
				cBox->setCurrentIndex(value);
				break;
			}

		case Types::Int:
			{
				int value = index.model()->data(index, Qt::EditRole).toInt();
				QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
				spinBox->setValue(value);
				break;
			}

		case Types::Double:
		case Types::Currency:
			{
				double value = index.model()->data(index, Qt::EditRole).toDouble();
				QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
				spinBox->setValue(value);
				break;
			}

		default:
			{
				QString value = index.model()->data(index, Qt::EditRole).toString();
				QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
				lineEdit->setText(value);
			}
	}
}


void ViewDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
		const QModelIndex &index) const
{
	int columnType = index.data(Types::TypeRole).toInt();
	switch(columnType) {
		case Types::Boolean:
			{
				QComboBox *cBox = static_cast<QComboBox*>(editor);
				int value = cBox->currentIndex();
				model->setData(index, value);
				break;
			}

		case Types::Int:
			{
				QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
				spinBox->interpretText();
				int value = spinBox->value();
				model->setData(index, value);
				break;
			}

		case Types::Double:
		case Types::Currency:
			{
				QDoubleSpinBox *spinBox = static_cast<QDoubleSpinBox*>(editor);
				spinBox->interpretText();
				double value = spinBox->value();
				model->setData(index, value);
				break;
			}

		default:
			{
				QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);
				QString value = lineEdit->text();

				model->setData(index, value);
				break;
			}
	}
}

void ViewDelegate::updateEditorGeometry(QWidget *editor,
		const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
	editor->setGeometry(option.rect);
}
