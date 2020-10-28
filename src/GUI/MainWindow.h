/*
 * MainWindow.h
 *
 *  Created on: 27 Sep 2020
 *      Author: Sven Rieper
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
#include "CentrePane.h"

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
	CentrePane centrePaneBox;

	Gtk::Frame topPaneBox;
	Gtk::Frame buttomPaneBox;
//	Gtk::Frame centerPaneBox;

	void fillWindow();
	void onWindowResize();
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_MAINWINDOW_H_ */
