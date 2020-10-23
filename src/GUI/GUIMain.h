/*
 * MainWindow.h
 *
 *  Created on: 27 Sep 2020
 *      Author: Sven Rieper
 */

#ifndef SRC_GUI_GUIMAIN_H_
#define SRC_GUI_GUIMAIN_H_

#include "../Backend/BackendFactory.h"

namespace PhotoLibrary {
namespace GUI {

int drawMainWindow(int argc, char* argv[], Backend::BackendFactory* backend);

} /* namespace GUI */
} /* namespace PhotoLibrary */

#endif /* SRC_GUI_GUIMAIN_H_ */
