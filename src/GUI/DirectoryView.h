/*
 * DirectoryView.h
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

#ifndef SRC_GUI_DIRECTORYVIEW_H_
#define SRC_GUI_DIRECTORYVIEW_H_

#include "BaseTreeView.h"
#include "DirectoryStore.h"

namespace PhotoLibrary {
namespace GUI {

/**
 * TreeView for the dierectories.
 * BaseTreeView based TreeView for the directories for the side pane.
 */
class DirectoryView: public BaseTreeView<DirectoryStore, Backend::DirectoryRecord> {
public:
	/**
	 * @param backend the backend interface factory object
	 */
	DirectoryView(Backend::BackendFactory* backend);
	virtual ~DirectoryView() = default;

private:
	void createView() override;
};

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_DIRECTORYVIEW_H_ */
