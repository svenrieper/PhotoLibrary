/*
 * EditKeywordDialogue.h
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

#ifndef SRC_GUI_EDITKEYWORDDIALOGUE_H_
#define SRC_GUI_EDITKEYWORDDIALOGUE_H_

#include <gtkmm/dialog.h>
#include <gtkmm/entry.h>
#include <gtkmm/checkbutton.h>

#include "../Backend/Record/KeywordRecord.h"

namespace PhotoLibrary {
namespace GUI {

/**
 * The Dialogue to edit keywords.
 *
 * @see https://developer.gnome.org/gtkmm/stable/classGtk_1_1Dialog.html
 */
class EditKeywordDialogue: public Gtk::Dialog {
public:
	/**
	 * @param[in,out] keyword Pointer to the Backend::KeywordRecord object to edit
	 */
	EditKeywordDialogue(Backend::RecordClasses::KeywordRecord* keyword);
	virtual ~EditKeywordDialogue() = default;

	//no copying or moving
	EditKeywordDialogue(const EditKeywordDialogue &other) = delete;
	EditKeywordDialogue(EditKeywordDialogue &&other) = delete;
	EditKeywordDialogue& operator=(const EditKeywordDialogue &other) = delete;
	EditKeywordDialogue& operator=(EditKeywordDialogue &&other) = delete;

protected:
	Backend::RecordClasses::KeywordRecord* keyword;	/**< Pointer to the Backend::Keyword that is edited */

private:
	Gtk::Label keyword_label;
	Gtk::Entry keyword_entry;
	Gtk::Label synonyms_label;
	Gtk::Entry synonyms_entry;
	Gtk::CheckButton check_button_private;
	Gtk::CheckButton check_button_export;
	Gtk::CheckButton check_button_export_synonyms;

	void updateKeyword();
	void updateSynonyms();
	void onPrivateClicked();
	void onExportClicked();
	void onExportSynonymsClicked();
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_EDITKEYWORDDIALOGUE_H_ */
