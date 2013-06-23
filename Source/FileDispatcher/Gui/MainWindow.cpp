/********************************************************************
	created:	2013/06/16
	created:	16:6:2013   13:20
	filename: 	\FileDispatcher\MainWindow.cpp
	file path:	\FileDispatcher
	file base:	MainWindow
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <QtWidgets/QApplication>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMenu>

#include "Page.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent /* = 0 */, Qt::WindowFlags flags /* = 0 */)
    : QMainWindow(parent, flags)
{
    setupMenu();
    setCentralWidget(new Page(this));

    setWindowTitle("File Dispatcher");
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupMenu()
{
    mpMenuFile = new QMenu(QString::fromLocal8Bit("&File"), this);

    mpMenuFile->addAction(QString::fromLocal8Bit("&Exit"), this, SLOT(slotExit()));

    menuBar()->addMenu(mpMenuFile);

    mpMenuHelp = new QMenu(QString::fromLocal8Bit("&Help"), this);

    mpMenuHelp->addAction(QString::fromLocal8Bit("&About"), this, SLOT(slotAbout()));

    menuBar()->addMenu(mpMenuHelp);
}

void MainWindow::slotExit()
{
    QApplication::exit();
}
