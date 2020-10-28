/*
 * MainWindow.cpp
 *
 *  Created on: 27 Sep 2020
 *      Author: Sven Rieper
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
		leftPaneBox(backend),
		centrePaneBox(backend) {
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

	rightPane.pack1(centrePaneBox, true, false);
	rightPane.pack2(rightPaneBox, false, false);

	topPaneBox.set_size_request(-1, 50);
	buttomPaneBox.set_size_request(-1, 150);
	leftPaneBox.set_size_request(150, -1);
	rightPaneBox.set_size_request(150, -1);
	centrePaneBox.set_size_request(250, 250);

	topPaneBox.set_shadow_type(Gtk::SHADOW_NONE);
	buttomPaneBox.set_shadow_type(Gtk::SHADOW_NONE);
	leftPaneBox.set_shadow_type(Gtk::SHADOW_NONE);
	rightPaneBox.set_shadow_type(Gtk::SHADOW_NONE);
	centrePaneBox.set_shadow_type(Gtk::SHADOW_NONE);
}

/// \todo find a solution that isn't called every time any child widget changes
void MainWindow::onWindowResize() {
	backend->setWindowProperty(Backend::BackendFactory::WindowProperties::WINDOW_WIDTH, get_width());
	backend->setWindowProperty(Backend::BackendFactory::WindowProperties::WINDOW_HEIGHT, get_height());
}


} /* namespace GUI */
} /* namespace PhotoLibrary */
