//
// Created by root on 18-2-6.
//

#include "SQLiteOpenHelper.h"

SQLiteOpenHelper::SQLiteOpenHelper() {
    fprintf(stdout, "SQLiteOpenHelper(): %p\n", this);
}

SQLiteOpenHelper::SQLiteOpenHelper(char *dbFilePath) {
    fprintf(stdout, "SQLiteOpenHelper(char*): %p\n", this);
    this->mSQLiteDatabaseFilePath = dbFilePath;
}

SQLiteOpenHelper::~SQLiteOpenHelper() {
    fprintf(stdout, "~SQLiteOpenHelper(): %p\n", this);
    if (this->mSQLiteDatabase != NULL) {
        sqlite3_close(mSQLiteDatabase);
        this->mSQLiteDatabase = NULL;
    }
}

void SQLiteOpenHelper::onCreate() {
    this->sqlite3FD = sqlite3_open(
            this->mSQLiteDatabaseFilePath, &mSQLiteDatabase);
    if (this->sqlite3FD) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(mSQLiteDatabase));
        exit(0);
    }
    // 打开数据库成功时返回"0"
    fprintf(stdout, "Open database success.\n");
}

void SQLiteOpenHelper::onUpgrade() {

}

void SQLiteOpenHelper::setSQLiteDatabaseFilePath(char *dbFilePath) {
    this->mSQLiteDatabaseFilePath = dbFilePath;
}