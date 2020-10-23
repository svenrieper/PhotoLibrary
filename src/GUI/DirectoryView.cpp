/*
 * DirectoryView.cpp
 *
 *  Created on: 15 Oct 2020
 *      Author: Sven Rieper
 */

#include "DirectoryView.h"

namespace PhotoLibrary {
namespace GUI {

DirectoryView::DirectoryView(Backend::BackendFactory* backend) :
		BaseTreeView(backend->getDirectoriesInterface()) {
	unset_rows_drag_source();
	unset_rows_drag_dest();

	createView();
//	getTreeStore()->signalExpandRow().connect(sigc::mem_fun(*this, &DirectoryView::onSignalExpandRow));
//	getTreeStore()->initialise();

//	connectOnRowExpandedOrCollapsed();
}

void DirectoryView::createView() {
	DirectoryStore::ModelColumns& columns = getTreeStore()->getColumns();
	//Add the TreeView's view columns:
	append_column("Directory", columns.name);
}

} /* namespace GUI */
} /* namespace PhotoLibrary */
