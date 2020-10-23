/*
 * main.cpp
 *
 *  Created on: 27 Sep 2020
 *      Author: Sven Rieper
 */

#include "Adapter/DatabaseFactory.h"
#include "Backend/BackendFactory.h"
#include "GUI/GUIMain.h"

using PhotoLibrary::Backend::BackendFactory;
using PhotoLibrary::GUI::drawMainWindow;

int main(int argc, char* argv[]) {
	BackendFactory backend(":memory:");

	return drawMainWindow(argc, argv, &backend);
}






