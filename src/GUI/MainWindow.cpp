/*
 * MainWindow.cpp
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

#include "MainWindow.h"

namespace PhotoLibrary {
namespace GUI {

MainWindow::MainWindow(Backend::BackendFactory* backend) :
		Gtk::Window(),
		backend(backend),
		topPane(Gtk::ORIENTATION_VERTICAL),
		buttomPane(Gtk::ORIENTATION_VERTICAL),
		leftPane(Gtk::ORIENTATION_HORIZONTAL),
		rightPane(Gtk::ORIENTATION_HORIZONTAL),
		rightPaneBox(backend),
		leftPaneBox(backend) {
//	set_hide_titlebar_when_maximized(true);
//	set_mnemonics_visible(true);
	set_title("Photo Library");

	fillWindow();

	set_default_size(backend->getWindowProperty(Backend::BackendFactory::WindowProperties::WINDOW_WIDTH),
			backend->getWindowProperty(Backend::BackendFactory::WindowProperties::WINDOW_HEIGHT));
	leftPane.property_position() = backend->getWindowProperty(Backend::BackendFactory::WindowProperties::LEFT_PANE_WIDTH);
	rightPane.property_position() = backend->getCentreWidth();

	show_all_children();

	signal_check_resize().connect(sigc::mem_fun(*this,&MainWindow::onWindowResize));
}

void MainWindow::fillWindow() {
	add(topPane);

	topPane.pack1(topPaneBox, false, false);
	topPane.pack2(buttomPane, true, true);

	buttomPane.pack1(leftPane, true, true);
	buttomPane.pack2(buttomPaneBox, false, false);

	leftPane.pack1(leftPaneBox, false, false);
	leftPane.pack2(rightPane, true, true);

	rightPane.pack1(centerPaneBox, true, false);
	rightPane.pack2(rightPaneBox, false, false);

	topPaneBox.set_size_request(-1, 50);
	buttomPaneBox.set_size_request(-1, 150);
	leftPaneBox.set_size_request(150, -1);
	rightPaneBox.set_size_request(150, -1);
	centerPaneBox.set_size_request(250, 250);

	topPaneBox.set_shadow_type(Gtk::SHADOW_NONE);
	buttomPaneBox.set_shadow_type(Gtk::SHADOW_NONE);
	leftPaneBox.set_shadow_type(Gtk::SHADOW_NONE);
	rightPaneBox.set_shadow_type(Gtk::SHADOW_NONE);
	centerPaneBox.set_shadow_type(Gtk::SHADOW_NONE);
}

/// \todo find a solution that isn't called every time any child widget changes
void MainWindow::onWindowResize() {
	int width, height;
	get_size(width, height);
	backend->setWindowProperty(Backend::BackendFactory::WindowProperties::WINDOW_WIDTH, width);
	backend->setWindowProperty(Backend::BackendFactory::WindowProperties::WINDOW_HEIGHT, height);
}


} /* namespace GUI */
} /* namespace PhotoLibrary */
