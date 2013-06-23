/********************************************************************
	created:	2013/06/16
	created:	16:6:2013   16:29
	filename: 	\FileDispatcher\FileInfoWidget.h
	file path:	\FileDispatcher
	file base:	FileInfoWidget
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _FILE_DISPATCHER_FILE_INFO_WIDGET_H_
#define _FILE_DISPATCHER_FILE_INFO_WIDGET_H_

#include <QtWidgets/QWidget>

class QItemSelection;
class QLabel;
class QResizeEvent;

class FileInfoWidget : public QWidget
{
    Q_OBJECT
public:
    FileInfoWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);
    ~FileInfoWidget();
protected:
    void setupUi();
protected slots:
    void slotSelectionChanged(const QItemSelection&, const QItemSelection&);
protected:
    QLabel* mpImage;
    QLabel* mpAbsolutePath;
    QLabel* mpPermissions;
    QLabel* mpSize;
    QPixmap transparentImage;

    static QSize imageSize;
};

#endif
