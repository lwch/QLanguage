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
#include <QtCore/QItemSelection>

#include <QtGui/QPicture>

#include <QtWidgets/QFileSystemModel>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

#include "../Data/DataManager.h"

#include "Page.h"
#include "FileList.h"
#include "FileInfoWidget.h"

QSize FileInfoWidget::imageSize(300, 250);

FileInfoWidget::FileInfoWidget(QWidget* parent /* = 0 */, Qt::WindowFlags flags /* = 0 */)
    : QWidget(parent, flags)
    , transparentImage(imageSize)
{
    setupUi();

    transparentImage.fill(Qt::transparent);
}

FileInfoWidget::~FileInfoWidget()
{
}

void FileInfoWidget::setupUi()
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);

    mpImage = new QLabel(this);
    mpImage->setFixedSize(imageSize);
    mpImage->setStyleSheet("border-width: 1px; border-style: solid;");
    mpImage->setAlignment(Qt::AlignCenter);
    pLayout->addWidget(mpImage);

    mpAbsolutePath = new QLabel("File Path: ", this);
    mpAbsolutePath->setFixedWidth(imageSize.width() + 100);
    pLayout->addWidget(mpAbsolutePath);

    mpPermissions = new QLabel("Permissions: ", this);
    pLayout->addWidget(mpPermissions);

    mpSize = new QLabel("Size: ", this);
    pLayout->addWidget(mpSize);

    pLayout->setAlignment(Qt::AlignCenter);

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
            if (info.isFile() && qPrintable(info.absoluteFilePath()))
            {
                QPixmap pixmap(info.absoluteFilePath());
                double dScale = pixmap.width() > pixmap.height() ? (double)imageSize.width() / pixmap.width() : (double)imageSize.height() / pixmap.height();
                dScale *= 0.9;
                mpImage->setPixmap(pixmap.scaled(pixmap.width() * dScale, pixmap.height() * dScale));
            }
            else mpImage->setPixmap(transparentImage);

            mpAbsolutePath->setText(QString("File Path: %1").arg(info.absoluteFilePath()));
            mpPermissions->setText(QString("Permissions: %1").arg(info.permissions(), 0, 16));
            mpSize->setText(QString("Size: %1").arg(info.size()));
        }
    }
}
