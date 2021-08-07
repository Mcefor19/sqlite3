#include <QtCore/QCoreApplication>
#include <iostream>
#include "sqlReader.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

	string yourDBFilePath = "C:\\sqlTest.db", yourTableName = "testTable";
	sqlReader reader(yourDBFilePath);
	if (reader.open())
	{
		cout << reader.sqliteVersion() << endl;
		QList<string> yourTableFieldNames{ "TEST1","TEST2" };
		reader.insertContentArgs(yourTableName, yourTableFieldNames, "age", 18);
		reader.insertContentArgs(yourTableName, "hello","world");
		reader.close();
	}

    return a.exec();
}
