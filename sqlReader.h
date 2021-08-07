#pragma once
#include <QtCore>
#include <string>
#include <sstream>
#include "sqlite3.h"


using namespace std;

class sqlReader
{
public:
	void close();
	string sqliteVersion();

	virtual bool open();
	virtual bool isOpen();				//判断数据库是否是打开状态
	virtual void beginWork();			//开启事务
	virtual void closeWork();			//关闭事务
	virtual void setSynchronous();		//开启写同步
	virtual void resetSynchronous();	//开启写同步

	sqlReader(string path);
	virtual ~sqlReader();

	template <typename T>
	void insertToList(QList<string>& list, T t)
	{
		stringstream ss;
		ss << forward<T>(t);
		list.append(ss.str());
	}

	template<typename ...Args>
	QList<string> expandList(Args...args)
	{
		QList<string> list;
		int arr[] = { (insertToList(list,forward<Args>(args)),0)... };     //逗号展开，分割参数进行处理
		return list;
	}

	//插入数据，tableName：表名，后续形参为可变参数，所有可变参数都会转为string类型
	template<typename ...Args>
	int insertContentArgs(string tableName, Args...args)
	{
		QList<string> list;
		list = expandList(args...);
		return insertContent(tableName, list);
	}

	//插入数据，tableName：表名，fieldName：形参名，后续形参为可变参数，所有可变参数都会转为string类型
	template<typename...Args>
	int insertContentArgs(string tableName, QList<string> fieldName,Args...args)
	{
		QList<string> list;
		list = expandList(args...);
		return insertContent(tableName, fieldName,list);
	}

	//该函数内部进行了事务开启以及存储准备，存储速度更快.一般的，需要用户将存储内容自行转为QList<string>形式，然后再存储到一个QList
	//容器中，然后可以间隔一段时间进行调用该函数
	int insertContentByExecPreparation(string tableName, QList<QList<string>> content);

protected:
	string _DBPath;
	int insertContent(string tableName, QList<string> fieldName, QList<string> content);
	int insertContent(string tableName, QList<string> content);

private:
	sqlite3* _DB{ nullptr };
};