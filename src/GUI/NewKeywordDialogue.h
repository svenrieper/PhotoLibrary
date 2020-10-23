/*
 * NewKeywordDialogue.h
 *
 *  Created on: 6 Oct 2020
 *      Author: Sven Rieper
 */

#ifndef SRC_GUI_NEWKEYWORDDIALOGUE_H_
#define SRC_GUI_NEWKEYWORDDIALOGUE_H_

#include "../Backend/Record/KeywordRecord.h"
#include "EditKeywordDialogue.h"

namespace PhotoLibrary {
namespace GUI {

/**
 * The Dialogue to add new keywords
 */
class NewKeywordDialogue: public EditKeywordDialogue {
public:
	/**
	 * @param[out] keyword Pointer to the Backend::NewKeywordRecord object to save the to
	 * @param[in] parent_key The parent keyword
	 */
	NewKeywordDialogue(Backend::NewKeywordRecord* keyword, const Glib::ustring& parent_key="");
	virtual ~NewKeywordDialogue() = default;

	//no copying or moving
	NewKeywordDialogue(const NewKeywordDialogue &other) = delete;
	NewKeywordDialogue(NewKeywordDialogue &&other) = delete;
	NewKeywordDialogue& operator=(const NewKeywordDialogue &other) = delete;
	NewKeywordDialogue& operator=(NewKeywordDialogue &&other) = delete;

private:
	Gtk::CheckButton check_button_add_as_child;

	void onAddAsChildClicked();
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_NEWKEYWORDDIALOGUE_H_ */
