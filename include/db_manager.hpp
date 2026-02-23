#pragma once
#include<string>
#include<vector>
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/VariadicBind.h>

#include"models.hpp"


class db_manager
{
    private:
    
    public:
        db_manager(){}
        ~db_manager(){}

        void create_relations(SQLite::Database& db);

        void register_user(User info,SQLite::Database& db);
        User get_user(string uname,SQLite::Database& db);
        bool change_user_password(string uname,string new_pass,SQLite::Database& db);
        bool delete_user(string uname,SQLite::Database& db);

        bool insert_password(PasswordStore info,User user_info,SQLite::Database& db);
        bool change_password(PasswordStore old_info,EncryptedData new_pass_info,SQLite::Database& db);
        bool delete_password(PasswordStore info,SQLite::Database& db);
        PasswordStore view_password(string site,SQLite::Database& db);
        std::vector<PasswordStore> view_passwords(SQLite::Database& db);
};


