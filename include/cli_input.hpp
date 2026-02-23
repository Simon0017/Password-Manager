#pragma once
#include<string>

#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/VariadicBind.h>

using namespace std;

class InputManager
{
private:
    
public:
    InputManager(){}
    ~InputManager(){}

    enum class Action{
        LOGIN,
        REGISTER,
        GET_PASSWORD,
        INSERT_PASSWORD,
        CHANGE_PASSWORD,
        DELETE_PASSWORD,
        VIEW_PASSWORDS,
        CHANGE_USER_PASSWORD,
        DELETE_USER,
        BACK,
        EXIT
    };

    Action get_user_action(string input);
    void handle_user_action(Action action, SQLite::Database& db);
};

string get_user_input(string prompt);
void login(SQLite::Database& db);
void register_user(SQLite::Database& db);
void get_password(SQLite::Database& db);
void insert_password(SQLite::Database& db);
void change_password(SQLite::Database& db);
void delete_password(SQLite::Database& db);
void view_passwords(SQLite::Database& db);
void change_user_password(SQLite::Database& db);
void delete_user(SQLite::Database& db);
void exit_program();
void back_to_menu();
void display_menu();