/********************************************************************
	created:	2013/06/23
	created:	23:6:2013   13:54
	filename: 	\FileDispatcher\Data\DataManager.cpp
	file path:	\FileDispatcher\Data
	file base:	DataManager
	file ext:	cpp
	author:		lwch
	
	purpose:	
*********************************************************************/
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>

#include "DataManager.h"

DataManager gDataManager;

DataManager::DataManager()
    : conn(QSqlDatabase::addDatabase("QSQLITE"))
{
    conn.setDatabaseName("test.db");
    conn.open();
}

DataManager::~DataManager()
{
    if (conn.isOpen()) conn.close();
}

QSqlQuery DataManager::exec(const QString& query)const
{
    if (conn.isOpen()) return conn.exec(query);
    return QSqlQuery();
}

bool DataManager::createTable(const QString& tableName, const QString& parameters)
{
    return QSqlQuery(QString("CREATE TABLE IF NOT EXISTS %1(%2)").arg(tableName).arg(parameters)).exec();
}
