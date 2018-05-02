//
// Created by root on 18-2-6.
//

#ifndef TCP_IP_SERVER_SQLITEOPENHELPER_H
#define TCP_IP_SERVER_SQLITEOPENHELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

class SQLiteOpenHelper {
private:
    sqlite3 *mSQLiteDatabase;
    int sqlite3FD;
    char *mSQLiteDatabaseFilePath;
public:
    SQLiteOpenHelper();

    SQLiteOpenHelper(char *dbFilePath);

    ~SQLiteOpenHelper();

    void onCreate();

    void onUpgrade();

    void setSQLiteDatabaseFilePath(char *dbFilePath);
};


#endif //TCP_IP_SERVER_SQLITEOPENHELPER_H
