/********************************************************************
	created:	2013/06/16
	created:	16:6:2013   15:31
	filename: 	\FileDispatcher\FileList.cpp
	file path:	\FileDispatcher
	file base:	FileList
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <QtWidgets/QFileSystemModel>
#include <QtWidgets/QHeaderView>
#include "FileList.h"

FileList::FileList(QWidget* parent /* = 0 */)
    : QTreeView(parent)
{
    QFileSystemModel* pModel = new QFileSystemModel(this);
    pModel->setRootPath(QDir::homePath());
    setModel(pModel);

    setSelectionMode(QAbstractItemView::SingleSelection);
    header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

FileList::~FileList()
{
}
