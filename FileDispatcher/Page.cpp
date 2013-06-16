/********************************************************************
	created:	2013/06/16
	created:	16:6:2013   15:41
	filename: 	\FileDispatcher\Page.cpp
	file path:	\FileDispatcher
	file base:	Page
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>

#include "FileList.h"
#include "FileInfoWidget.h"
#include "Page.h"

Page::Page(QWidget* parent /* = 0 */, Qt::WindowFlags flags /* = 0 */)
    : QWidget(parent, flags)
{
    setupUi();
}

Page::~Page()
{
}

void Page::setupUi()
{
    QVBoxLayout* pLayout = new QVBoxLayout(this);
    pLayout->setSpacing(0);
    pLayout->setMargin(0);

    QLineEdit* pSearchEdit = new QLineEdit(this);
    pLayout->addWidget(pSearchEdit);

    QGridLayout* pGridLayout = new QGridLayout(this);

    mpFileList = new FileList(this);
    pGridLayout->addWidget(mpFileList, 0, 0);

    mpFileInfoWidget = new FileInfoWidget(this);
    pGridLayout->addWidget(mpFileInfoWidget, 0, 1);

    connect(mpFileList->selectionModel(), SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), mpFileInfoWidget, SLOT(slotSelectionChanged(const QItemSelection&, const QItemSelection&)));

    // 3:1
    pGridLayout->setColumnStretch(0, 3);
    pGridLayout->setColumnStretch(1, 1);
    pLayout->addLayout(pGridLayout);

    setLayout(pLayout);
}
