/*
 * EditKeywordDialogue.cpp
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

#include "EditKeywordDialogue.h"

#include <iostream>

namespace PhotoLibrary {
namespace GUI {

using Backend::RecordClasses::KeywordRecord;

EditKeywordDialogue::EditKeywordDialogue(KeywordRecord *keyword) :
		/// \todo add support for internationalisation
		keyword(keyword),
		keyword_label("Keyword:"),
		synonyms_label("Synonyms:"),
		check_button_private("Private keyword"),
		check_button_export("Include keyword on export"),
		check_button_export_synonyms("Export synonyms") {
	//Set the window properties
	set_title("New Keyword");
	set_resizable(false);
	set_default_size(550, -1);
	set_border_width(4);

	//Set the values of the widgets
	keyword_entry.set_text(keyword->getKeyword());
	synonyms_entry.set_text(keyword->getSynonyms());
	check_button_private.set_active(keyword->getOptions() & KeywordRecord::Options::PRIVATE);
	check_button_export.set_active(keyword->getOptions() & KeywordRecord::Options::INCLUDE_ON_EXPORT);
	check_button_export_synonyms.set_active(keyword->getOptions() & KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT);

	//Fill the window with widgets
	add_button("_OK", Gtk::RESPONSE_OK);
	add_button("_Cancel", Gtk::RESPONSE_CANCEL);

	Gtk::Widget* ok_button = get_widget_for_response(Gtk::RESPONSE_OK);
	if(ok_button) {
		ok_button->set_can_default();
		ok_button->grab_default();
	}

	keyword_label.set_xalign(0);
	synonyms_label.set_xalign(0);
	keyword_entry.set_activates_default();
	synonyms_entry.set_activates_default();

	Gtk::Box *content_box = get_content_area();

	content_box->add(keyword_label);
	content_box->add(keyword_entry);
	content_box->add(synonyms_label);
	content_box->add(synonyms_entry);
	content_box->add(check_button_private);
	content_box->add(check_button_export);
	content_box->add(check_button_export_synonyms);
	content_box->show_all_children(true);

	//Connect the signal handlers
	keyword_entry.signal_changed().connect(sigc::mem_fun(*this, &EditKeywordDialogue::updateKeyword));
	synonyms_entry.signal_changed().connect(sigc::mem_fun(*this, &EditKeywordDialogue::updateSynonyms));
	check_button_private.signal_clicked().connect(sigc::mem_fun(*this, &EditKeywordDialogue::onPrivateClicked));
	check_button_export.signal_clicked().connect(sigc::mem_fun(*this, &EditKeywordDialogue::onExportClicked));
	check_button_export_synonyms.signal_clicked().connect(sigc::mem_fun(*this, &EditKeywordDialogue::onExportSynonymsClicked));
}

void EditKeywordDialogue::updateKeyword() {
	keyword->setKeyword() = keyword_entry.get_text();
}

void EditKeywordDialogue::updateSynonyms() {
	keyword->setSynonyms() = synonyms_entry.get_text();
}

void EditKeywordDialogue::onPrivateClicked() {
	keyword->setOptions() ^= KeywordRecord::Options::PRIVATE;
}

void EditKeywordDialogue::onExportClicked() {
	keyword->setOptions() ^= KeywordRecord::Options::INCLUDE_ON_EXPORT;
}

void EditKeywordDialogue::onExportSynonymsClicked() {
	keyword->setOptions() ^= KeywordRecord::Options::INCLUDE_SYNONYMS_ON_EXPORT;
}

} /* namespace GUI */
} /* namespace PhotoLibrary */
