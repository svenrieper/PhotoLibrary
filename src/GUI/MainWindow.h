/*
 * MainWindow.h
 *
 * This file is part of PhotoLibrary
 * Copyright (C) 2020 Sven Rieper
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef SRC_MAINWINDOW_H_
#define SRC_MAINWINDOW_H_

#include <gtkmm/window.h>
#include <gtkmm/paned.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>

#include "RightPane.h"
#include "LeftPane.h"

namespace PhotoLibrary {
namespace GUI {

/**
 * Provides the base structure of the main window.
 */
class MainWindow: public Gtk::Window {
public:
	/**
	 * @param backend Pointer to the backend factory object
	 */
	MainWindow(Backend::BackendFactory* backend);
	~MainWindow() = default;

private:
	Backend::BackendFactory* backend;

	//Child Widgets
	Gtk::Paned topPane;
	Gtk::Paned buttomPane;
	Gtk::Paned leftPane;
	Gtk::Paned rightPane;

	RightPane rightPaneBox;
	LeftPane leftPaneBox;

	Gtk::Frame topPaneBox;
	Gtk::Frame buttomPaneBox;
	Gtk::Frame centerPaneBox;

	void fillWindow();
	void onWindowResize();
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_MAINWINDOW_H_ */
