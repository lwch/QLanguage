/********************************************************************
	created:	2013/06/16
	created:	16:6:2013   12:40
	filename: 	\FileDispatcher\main.cpp
	file path:	\FileDispatcher
	file base:	main
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <QtWidgets/QApplication>

#include "Gui/MainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow window;
    window.showMaximized();

    return app.exec();
}
