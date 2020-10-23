/*
 * DirectoryView.h
 *
 *  Created on: 15 Oct 2020
 *      Author: Sven Rieper
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
