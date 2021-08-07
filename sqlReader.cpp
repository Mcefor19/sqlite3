#include "sqlReader.h"

sqlReader::sqlReader(string path) :_DBPath(path)
{

}

sqlReader::~sqlReader()
{
	close();
}

bool sqlReader::open()
{
	if (sqlite3_open_v2(_DBPath.c_str(), &_DB, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_NOMUTEX | SQLITE_OPEN_SHAREDCACHE, nullptr) == SQLITE_OK)
		return true;
	return false;
}

void sqlReader::close()
{
	if (_DB)
	{
		sqlite3_close(_DB);
		_DB = nullptr;
	}
}

string sqlReader::sqliteVersion()
{
	return sqlite3_libversion();
}

bool sqlReader::isOpen()
{
	return _DB == nullptr ? false : true;
}

int sqlReader::insertContent(string tableName, QList<string> fieldName, QList<string> content)
{
	if (_DB == nullptr)
		return -1;
	string filedParam = "", contentParam = "";
	for (int i = 0; i < fieldName.size(); ++i)
	{
		filedParam += fieldName.at(i);
		if (i != (content.size() - 1))
			filedParam += ",";
	}
	for (int i = 0; i < content.size(); ++i)
	{
		contentParam += "'", contentParam += content.at(i), contentParam += "'";
		if (i != (content.size() - 1))
			contentParam += ",";
	}
	stringstream ssm;
	ssm << "INSERT INTO " << tableName << "(" << filedParam << ")" << " VALUES(" << contentParam << ")";
	return sqlite3_exec(_DB, ssm.str().c_str(), 0, 0, 0);
}

int sqlReader::insertContent(string tableName, QList<string> content)
{
	if (_DB == nullptr)
		return -1;
	string param = "";
	for (int i = 0; i < content.size(); ++i)
	{
		param += "'", param += content.at(i), param += "'";
		if (i != (content.size() - 1))
			param += ",";
	}

	stringstream ssm;
	ssm << "INSERT INTO " << tableName << " VALUES(" << param << ")";
	return sqlite3_exec(_DB, ssm.str().c_str(), 0, 0, 0);
}

void sqlReader::beginWork()
{
	sqlite3_exec(_DB, "begin;", 0, 0, 0);
}

void sqlReader::closeWork()
{
	sqlite3_exec(_DB, "commit;", 0, 0, 0);
}

void sqlReader::setSynchronous()
{
	sqlite3_exec(_DB, "PRAGMA synchronous = OFF; ", 0, 0, 0);
}

void sqlReader::resetSynchronous()
{
	sqlite3_exec(_DB, "PRAGMA synchronous = ON; ", 0, 0, 0);
}


int sqlReader::insertContentByExecPreparation(string tableName, QList<QList<string>> content)
{
	if (_DB == nullptr || content.isEmpty() || content.at(0).isEmpty())
		return -1;
	string contentParam = "";
	for (int i = 0; i < content.at(0).size(); ++i)
	{
		contentParam += "?";
		if (i != (content.at(0).size() - 1))
			contentParam += ",";
	}

	stringstream ssm;
	ssm << "INSERT INTO " << tableName << " VALUES(" << contentParam << ")";
	sqlite3_exec(_DB, "begin;", 0, 0, 0);
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(_DB, ssm.str().c_str(), ssm.str().length(), &stmt, 0);
	for (int i = 0; i < content.size(); ++i)
	{
		long long count = 1;
		for (int n = 0; n < content.at(i).size(); ++n)
			sqlite3_bind_text(stmt, count++, content.at(i).at(n).c_str(), content.at(i).at(n).length(), nullptr);
		sqlite3_step(stmt);
		sqlite3_reset(stmt);
	}
	sqlite3_finalize(stmt);
	return sqlite3_exec(_DB, "commit;", 0, 0, 0);
}

