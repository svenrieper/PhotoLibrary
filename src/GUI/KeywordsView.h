/*
 * KeywordsView.h
 *
 *  Created on: 28 Sep 2020
 *      Author: Sven Rieper
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
