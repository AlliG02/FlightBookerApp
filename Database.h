#pragma once
#include <sqlite3.h>
#include <string>

class Database {
private:
    sqlite3* db;
    std::string dbName;

public:
    Database(const std::string& databaseName);
    ~Database();

    bool open();
    void close();
    bool execute(const std::string& sql);
    sqlite3* getDB() const;
};
