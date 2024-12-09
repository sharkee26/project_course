#include <iostream>
#include <string>
#include "sqlite3.h"

// Функция для обработки ошибок
void checkSqlError(int result) 
{
    if (result != SQLITE_OK) 
    {
        std::cerr << "Ошибка: " << sqlite3_errmsg(nullptr) << std::endl;
        exit(result);
    }
}

// Функция для создания таблицы
void createTable(sqlite3 *db) 
{
    const char *sql = "CREATE TABLE IF NOT EXISTS Users ("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "Name TEXT NOT NULL,"
                      "Age INTEGER NOT NULL);";
    
    char *errMsg;
    int result = sqlite3_exec(db, sql, nullptr, 0, &errMsg);
    if (result != SQLITE_OK) 
    {
        std::cerr << "Ошибка создания таблицы: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else 
    {
        std::cout << "Таблица создана успешно." << std::endl;
    }
}

// Функция для добавления записи
void insertUser(sqlite3 *db, const std::string &name, int age) 
{
    const char *sql = "INSERT INTO Users (Name, Age) VALUES (?, ?);";
    
    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (result != SQLITE_OK) 
    {
        std::cerr << "Ошибка подготовки SQL: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, age);

    result = sqlite3_step(stmt);
    
    if (result != SQLITE_DONE) 
    {
        std::cerr << "Ошибка выполнения SQL: " << sqlite3_errmsg(db) << std::endl;
    } else 
    {
        std::cout << "Пользователь добавлен успешно." << std::endl;
    }

    sqlite3_finalize(stmt);
}

// Функция для вывода данных из таблицы
void displayUsers(sqlite3 *db) 
{
    const char *sql = "SELECT * FROM Users;";
    
    sqlite3_stmt *stmt;
    int result = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    
    if (result != SQLITE_OK) 
    {
        std::cerr << "Ошибка подготовки SQL: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    while ((result = sqlite3_step(stmt)) == SQLITE_ROW) 
    {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char *name = sqlite3_column_text(stmt, 1);
        int age = sqlite3_column_int(stmt, 2);
        
        std::cout << "ID: " << id 
                  << ", Name: " << name 
                  << ", Age: " << age 
                  << std::endl;
    }

    if (result != SQLITE_DONE) 
    {
        std::cerr << "Ошибка выполнения SQL: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
}

int main() 
{
    sqlite3 *db;
    
    // Открытие базы данных
    int exit = sqlite3_open("test.db", &db);
    checkSqlError(exit);

    // Создание таблицы
    createTable(db);

    // Добавление пользователей
    insertUser(db, "Игорь", 30);
    insertUser(db, "Женя", 25);
    insertUser(db, "Женя", 27);

    // Вывод данных из таблицы
    displayUsers(db);

    // Закрытие базы данных
    sqlite3_close(db);
    
    return 0;
}
