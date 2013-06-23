/********************************************************************
	created:	2013/06/23
	created:	23:6:2013   13:54
	filename: 	\FileDispatcher\Data\DataManager.h
	file path:	\FileDispatcher\Data
	file base:	DataManager
	file ext:	h
	author:		lwch
	
	purpose:	
*********************************************************************/
#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include <QtSql/QSqlDatabase>

class DataManager
{
public:
    DataManager();
    ~DataManager();

    bool createTable(const QString& tableName, const QString& parameters);
protected:
    QSqlQuery exec(const QString& query)const;
protected:
    QSqlDatabase conn;
};

extern DataManager gDataManager;

#endif
