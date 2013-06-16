/********************************************************************
	created:	2013/06/16
	created:	16:6:2013   16:31
	filename: 	\FileDispatcher\FileInfoWidget.cpp
	file path:	\FileDispatcher
	file base:	FileInfoWidget
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtCore/QItemSelection>
#include <QtWidgets/QFileSystemModel>

#include "Page.h"
#include "FileList.h"
#include "FileInfoWidget.h"

FileInfoWidget::FileInfoWidget(QWidget* parent /* = 0 */, Qt::WindowFlags flags /* = 0 */)
    : QWidget(parent, flags)
{
    setupUi();
}

FileInfoWidget::~FileInfoWidget()
{
}

void FileInfoWidget::setupUi()
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);

    QLabel* pLabel = new QLabel("This is FileInfoWidget", this);
    pLayout->addWidget(pLabel);

    setLayout(pLayout);
}

void FileInfoWidget::slotSelectionChanged(const QItemSelection&, const QItemSelection&)
{
    Page* pPage = dynamic_cast<Page*>(parent());
    if (pPage)
    {
        QFileSystemModel* pModel = dynamic_cast<QFileSystemModel*>(pPage->mpFileList->model());
        if (pModel)
        {
            QFileInfo info = pModel->fileInfo(pPage->mpFileList->currentIndex());
            if (info.isFile())
            {
                qDebug(info.absoluteFilePath().toLocal8Bit().data());
            }
        }
    }
}
