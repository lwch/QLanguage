/********************************************************************
	created:	2013/06/16
	created:	16:6:2013   15:25
	filename: 	\FileDispatcher\FileList.h
	file path:	\FileDispatcher
	file base:	FileList
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _FILE_DISPATCHER_FILE_LIST_H_
#define _FILE_DISPATCHER_FILE_LIST_H_

#include <QtWidgets/QTreeView>

class FileList : public QTreeView
{
    Q_OBJECT
public:
    FileList(QWidget* parent = 0);
    ~FileList();
};

#endif
