/*
 * main.cpp
 *
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

#include "Adapter/DatabaseFactory.h"
#include "Backend/BackendFactory.h"
#include "GUI/GUIMain.h"

using PhotoLibrary::Backend::BackendFactory;
using PhotoLibrary::GUI::drawMainWindow;

int main(int argc, char* argv[]) {
	BackendFactory backend(":memory:");

	return drawMainWindow(argc, argv, &backend);
}






