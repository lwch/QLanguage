/********************************************************************
	created:	2013/06/16
	created:	16:6:2013   13:17
	filename: 	\FileDispatcher\MainWindow.h
	file path:	\FileDispatcher
	file base:	MainWindow
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _FILE_DISPATCHER_MAIN_WINDOW_H_
#define _FILE_DISPATCHER_MAIN_WINDOW_H_

#include <QtWidgets/QMainWindow>

class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~MainWindow();
protected:
    void setupMenu();
protected slots:
    void slotExit();
protected:
    QMenu*    mpMenuFile;
    QMenu*    mpMenuHelp;
};

#endif
