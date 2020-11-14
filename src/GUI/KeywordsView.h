/*
 * KeywordsView.h
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

#ifndef SRC_GUI_KEYWORDSVIEW_H_
#define SRC_GUI_KEYWORDSVIEW_H_

#include <gtkmm/treeview.h>
#include "KeywordsStore.h"
#include "BaseTreeView.h"

namespace PhotoLibrary {
namespace GUI {

/**
 * TreeView for the keywords.
 * BaseTreeView based TreeView for the keywords for the side pane.
 */
class KeywordsView: public BaseTreeView<KeywordsStore, Backend::KeywordRecord> {
public:
	/**
	 * @param backend the backend interface factory object
	 */
	KeywordsView(Backend::BackendFactory* backend);
	~KeywordsView() = default;

private:
	void createView() override;
	void fillPopupMenu();

	bool on_button_press_event(GdkEventButton* button_event) override;
	void onMenuEditKeyword();
	void onMenuAddNewKeyword();
	void onMenuDeleteKeyword();

	Gtk::Menu popup_menu;
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_KEYWORDSVIEW_H_ */
