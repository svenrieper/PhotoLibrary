/*
 * MainWindow.cpp
 *
 *  Created on: 27 Sep 2020
 *      Author: Sven Rieper
 */

#include "GUIMain.h"
#include "MainWindow.h"
#include <gtkmm/window.h>
#include <gtkmm/cssprovider.h>

//Fill the database with some examples during development
namespace PhotoLibrary {
void examples(PhotoLibrary::Backend::BackendFactory* db);
}

namespace PhotoLibrary {
namespace GUI {

void applyStyles(MainWindow* window);

int drawMainWindow(int argc, char *argv[], Backend::BackendFactory* backend) {
	//Fill the database with some examples during development
	examples(backend);

	auto app = Gtk::Application::create(argc, argv, "org.PhotoLibrary.main");

	MainWindow window(backend);
//	window.set_default_size(backend->getWindowProperty(Backend::BackendFactory::WindowProperties::WINDOW_WIDTH),
//			backend->getWindowProperty(Backend::BackendFactory::WindowProperties::WINDOW_HEIGHT));
	window.set_title("Photo Library");

	applyStyles(&window);

	return app->run(window);
}

void applyStyles(MainWindow* window) {
	const std::string styles =
			"window, scrolledWindow {"
			"	background-color:	#000000;"
			"	color:				#c8c8c8;"
			"	border-color:		#000000;"
			"	outline-color:		#000000;"
//			"	border-top-color:	#000000;"
			"}"
			"box {"
			"	background-color:	#202020;"
			"	color:				#c8c8c8;"
			"}"
			"menu {"
			"	background-color:	#404040;"
			"}"
			"menuitem:hover {"
			"	background-color:	#585858;"
			"	color:				#e0e0e0;"
			"}"
//			"headerbar, titlebar {"
//			"	background-color:	#585858;"
//			"	color:				#e0e0e0;"
//			"}"
//			"paned, paned seperator {"
//			"	color:	#181818;"
//			"}"
			"paned > separator {"
			"	border-color:		#383838;"
			"	border-style: 		solid;"
			"	border-width: 		0.0833333333333333em 0.0833333333333333em 0 0;"
//			"	margin: 0.3333333333333333em;"
//			"	padding: 0;"
			"}"
			"treeview {"
			"	background-color:	#303030;"
			"	color:				#b0b0b0;"
			"}"
//			"treeview.active {"
//			"	background-color:	#606060;"
//			"	color:				#c8c8c8;"
//			"}"
			"frame {"
			"	background-color:	#282048;"
			"}"
			"check {"
//			"	background-color:	#a0a0a0;"
			"	background-image:	linear-gradient(#343434, #2E2E2E, #292929);"	//CheckButton bachground
			"	border-top-color:	#202020;"
			"	border-right-color:	#202020;"
			"	border-left-color:	#202020;"
			"	border-bottom-color:	#202020;"
//			"	border-color:		#202020;"
			"	color:				#989898;"
			"}"
			"check:hover {"
//			"	background-color:	#a0a0a0;"
			"	background-image:	linear-gradient(#474747, #404040, #3b3b3b);"	//CheckButton bachground
			"	border-top-color:	#202020;"
			"	border-right-color:	#202020;"
			"	border-left-color:	#202020;"
			"	border-bottom-color:	#202020;"
//			"	border-color:		#202020;"
			"	color:				#989898;"
			"}"
			".view:selected {"
			"	background-color:	#484848;"
			"}"
			"#sidePaneElementTitle {"
			"	font-size:			15pt;"
			"}"
			"#sidePaneElementTitleHidden {"
			"	font-size:			15pt;"
			"	color:				#808080;"
			"}"
//			"checkbutton, checkbutton.active, checkbutton.check, button, bin, button:checked, button.check.entry, togglebutton, entry, .view {"
//			"	background-color:	#303030;"
//			"	outline-color:		#202020;"
//			"	caret-color:		#202020;"
//			"	-gtk-secondary-caret-color:	#202020;"
//			"	text-decoration-color:		#101010;"
////			"	border-top-color:	#202020;"
////			"	border-right-color:	#202020;"
////			"	border-left-color:	#202020;"
////			"	border-bottom-color:	#202020;"
//			"	color:				#c8c8c8;"
//			"}"
//			"checkbutton > check, button.flat:checked, button:checked, entry:selected, GtkCheckButton, GtkToggleButton.button.text-button:checked, .plainback {"
//			"	background-color:	#909090;"
//			"	background-image: linear-gradient(#343434, #2E2E2E, #292929);"
//			"}"
			;

	auto css_provider = Gtk::CssProvider::create();
	auto style_context = Gtk::StyleContext::create();

	css_provider->load_from_data(styles);
	style_context->add_provider_for_screen(window->get_screen(), css_provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
}


} /* namespace GUI */
} /* namespace PhotoLibrary */
