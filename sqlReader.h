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
	virtual bool isOpen();				//�ж����ݿ��Ƿ��Ǵ�״̬
	virtual void beginWork();			//��������
	virtual void closeWork();			//�ر�����
	virtual void setSynchronous();		//����дͬ��
	virtual void resetSynchronous();	//����дͬ��

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
		int arr[] = { (insertToList(list,forward<Args>(args)),0)... };     //����չ�����ָ�������д���
		return list;
	}

	//�������ݣ�tableName�������������β�Ϊ�ɱ���������пɱ��������תΪstring����
	template<typename ...Args>
	int insertContentArgs(string tableName, Args...args)
	{
		QList<string> list;
		list = expandList(args...);
		return insertContent(tableName, list);
	}

	//�������ݣ�tableName��������fieldName���β����������β�Ϊ�ɱ���������пɱ��������תΪstring����
	template<typename...Args>
	int insertContentArgs(string tableName, QList<string> fieldName,Args...args)
	{
		QList<string> list;
		list = expandList(args...);
		return insertContent(tableName, fieldName,list);
	}

	//�ú����ڲ��������������Լ��洢׼�����洢�ٶȸ���.һ��ģ���Ҫ�û����洢��������תΪQList<string>��ʽ��Ȼ���ٴ洢��һ��QList
	//�����У�Ȼ����Լ��һ��ʱ����е��øú���
	int insertContentByExecPreparation(string tableName, QList<QList<string>> content);

protected:
	string _DBPath;
	int insertContent(string tableName, QList<string> fieldName, QList<string> content);
	int insertContent(string tableName, QList<string> content);

private:
	sqlite3* _DB{ nullptr };
};