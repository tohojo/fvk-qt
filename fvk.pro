# This file is part of NatCafé FVK-system.
# Copyright (C) 2007 Toke Høiland-Jørgensen
#
# NatCafé FVK-system is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# NatCafé FVK-system is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

TEMPLATE += app
QT += sql
FORMS += ui/*.ui 
HEADERS += headers/constants.h \
	headers/types.h \
	headers/sqlhelpers.h \
	headers/dclineedit.h \
	headers/ordermodel.h \
	headers/itemsmodel.h \
	headers/usersmodel.h \
	headers/transactionsmodel.h \
	headers/viewdelegate.h \
	headers/connection.h \
	headers/addmoney.h \
	headers/adduser.h \
	headers/editusers.h \
	headers/edititems.h \
	headers/statistics.h \
	headers/statistics_total.h \
	headers/treeitem.h \
	headers/treemodel.h \
	headers/totalmodel.h \
	headers/itemstatsmodel.h \
	headers/mainwindow.h 
SOURCES += src/ordermodel.cpp \
	src/dclineedit.cpp \
	src/itemsmodel.cpp \
	src/usersmodel.cpp \
	src/transactionsmodel.cpp \
	src/viewdelegate.cpp \
	src/addmoney.cpp \
	src/adduser.cpp \
	src/editusers.cpp \
	src/edititems.cpp \
	src/statistics.cpp \
	src/statistics_total.cpp \
	src/treeitem.cpp \
	src/treemodel.cpp \
	src/totalmodel.cpp \
	src/itemstatsmodel.cpp \
	src/mainwindow.cpp \
	src/main.cpp 
CONFIG -= debug
CONFIG += qt release
DEFINES += QT_NO_DEBUG_OUTPUT
INCLUDEPATH += headers/
RESOURCES += fvk.qrc
unix:MOC_DIR = ./tmp
unix:OBJECTS_DIR = ./tmp
unix:UI_DIR = ./tmp
unix:RCC_DIR = ./tmp
