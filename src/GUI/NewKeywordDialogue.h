/*
 * NewKeywordDialogue.h
 *
 * This file is part of PhotoLibrary
 * Copyright (C) 2020-2021 Sven Rieper
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
	NewKeywordDialogue(Backend::RecordClasses::NewKeywordRecord* keyword, const Glib::ustring& parent_key="");
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
