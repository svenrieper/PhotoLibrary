/*
 * SidePaneElement.h
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

#ifndef SRC_GUI_SIDEPANEELEMENT_H_
#define SRC_GUI_SIDEPANEELEMENT_H_

#include <gtkmm/box.h>
#include <gtkmm/eventbox.h>
#include "../Backend/BackendFactory.h"

namespace PhotoLibrary {
namespace GUI {

/**
 * Base class for the elements shown in the left and right pane.
 *
 * @see https://developer.gnome.org/gtkmm/stable/classGtk_1_1Box.html
 *
 * \todo add 'add' button
 * \todo add search button and search function
 *
 * @param T Widget to show in the side pane (any widget that can be
 * 		placed inside a Gtk::Box)
 */
template<class T>
class SidePaneElement : public Gtk::Box {
public:
	/**
	 * @param title The title for the element, placed above the widget 'T'
	 * @param backend Pointer to the backend factory object
	 */
	SidePaneElement(Glib::ustring title, Backend::BackendFactory* backend);
	virtual ~SidePaneElement() = default;

private:
	Gtk::Box title_box;
	/// \todo use Gtk::Expander instead
	Gtk::EventBox title_label_box;
	Gtk::Label title_label;
//	Glib::RefPtr<Gtk::Button> add;
	T content;
	bool hidden;

	bool onClickLabel(GdkEventButton* button_event);
};

//implementation
template<class T>
SidePaneElement<T>::SidePaneElement(Glib::ustring title, Backend::BackendFactory* backend) :
		Gtk::Box(Gtk::ORIENTATION_VERTICAL),
		title_box(Gtk::ORIENTATION_HORIZONTAL),
		title_label(title),
		content(backend),
		hidden(false) {
	set_homogeneous(false);
	pack_start(title_box, Gtk::PACK_SHRINK, 6);
	pack_start(content, Gtk::PACK_SHRINK);

	title_box.pack_start(title_label_box, Gtk::PACK_EXPAND_WIDGET, 10);

	title_label_box.add(title_label);
	title_label_box.signal_button_press_event().connect(sigc::mem_fun(*this, &SidePaneElement<T>::onClickLabel));
	title_label.set_xalign(0);
	title_label.set_name("sidePaneElementTitle");
}

template<class T>
bool SidePaneElement<T>::onClickLabel(GdkEventButton* button_event) {
	if(hidden) {
		title_label.set_name("sidePaneElementTitle");
		content.show();
		hidden = false;
	}
	else {
		title_label.set_name("sidePaneElementTitleHidden");
		content.hide();
		hidden = true;
	}
	return true;
}

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_SIDEPANEELEMENT_H_ */
