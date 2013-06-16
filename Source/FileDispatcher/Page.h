/********************************************************************
	created:	2013/06/16
	created:	16:6:2013   15:36
	filename: 	\FileDispatcher\Page.h
	file path:	\FileDispatcher
	file base:	Page
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _FILE_DISPATCHER_PAGE_H_
#define _FILE_DISPATCHER_PAGE_H_

#include <QtWidgets/QWidget>

class FileList;
class FileInfoWidget;

class Page : public QWidget
{
    Q_OBJECT

    friend class FileInfoWidget;
public:
    Page(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~Page();
protected:
    void setupUi();
protected:
    FileList*       mpFileList;
    FileInfoWidget* mpFileInfoWidget;
};

#endif
