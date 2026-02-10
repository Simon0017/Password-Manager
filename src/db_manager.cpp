#include<string>
#include<vector>
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/VariadicBind.h>

#include"models.hpp"
#include"db_manager.hpp"

vector<unsigned char> read_blob(const void* blob,int size);

void db_manager::create_relations(SQLite::Database& db){
    try
    {
        db.exec(R"(
            CREATE TABLE IF NOT EXIST user(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            email TEXT,
            address TEXT,
            password_hash TEXT NOT NULL,
            );
        )");

        db.exec(R"(
            CREATE TABLE IF NOT EXIST password_store(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            site TEXT NOT NULL,
            encrypted_pwd BLOB NOT NULL,
            iv BLOB NOT NULL,
            created_at DATETIME DEFAULT CURRENT_TIMESTAMP,
            last_modified DATETIME DEFAULT CURRENT_TIMESTAMP,
            FOREIGN KEY(user_id) REFERENCES user(id)
            );
        )");
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}


User db_manager::register_user(User info,SQLite::Database& db){
    try
    {
        SQLite::Statement insertuser(
            db,
            "INSERT INTO user (username,email,address,password_hash) VALUES (?,?,?,?)"
        );

        insertuser.bind(1,info.username);
        insertuser.bind(2,info.email);
        insertuser.bind(3,info.address);
        insertuser.bind(4,info.password_hash);
        insertuser.exec();

        return info;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}


User db_manager::get_user(string uname,SQLite::Database& db){
    SQLite::Statement query(
        db,
        "SELECT id,username,email,address,password_hash FROM user WHERE username = ?"
    );

    query.bind(1,uname);

    User selected_user;

    if (query.executeStep())
    {
        selected_user.id = query.getColumn(0).getInt();
        selected_user.username = query.getColumn(1).getString();
        selected_user.email = query.getColumn(2).getString();
        selected_user.address = query.getColumn(3).getString();
        selected_user.password_hash = query.getColumn(4).getString();

        return selected_user;
    }

    return selected_user;
}


bool db_manager::change_user_password(string uname,string new_pass,SQLite::Database& db){
    try
    {
        SQLite::Statement update_pwd(
            db,
            "UPDATE user SET password_hash = ? WHERE username = ?"
        );

        update_pwd.bind(1,new_pass);
        update_pwd.bind(2,uname);
        update_pwd.exec();

        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
}


bool db_manager::delete_user(string uname,SQLite::Database& db){
    try
    {
        SQLite::Statement del_user(
            db,
            "DELETE FROM user WHERE username = ?"
        );

        del_user.bind(1,uname);
        del_user.exec();

        return true;

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
}


bool db_manager::insert_password(PasswordStore info,User user_info,SQLite::Database& db){
    try
    {
        SQLite::Statement insert_pwd(
            db,
            "INSERT INTO password_store (user_id,site,encrypted_pwd,iv) VALUES (?,?,?,?)"
        );

        insert_pwd.bind(1,user_info.id);
        insert_pwd.bind(2,info.site_url);
        insert_pwd.bind(3,info.pwdData.cipherText.data(),static_cast<int>(info.pwdData.cipherText.size()));
        insert_pwd.bind(4,info.pwdData.iv.data(),static_cast<int>(info.pwdData.iv.size()));

        insert_pwd.exec();

        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
}


bool db_manager::change_password(PasswordStore old_info,EncryptedData new_pass_info,SQLite::Database& db){
    try
    {
        SQLite::Statement change_pwd(
            db,
            "UPDATE password_store SET encrypted_pwd = ? AND iv = ? WHERE encrypted_pwd = ? "
        );

        change_pwd.bind(1,new_pass_info.cipherText.data(),static_cast<int>(new_pass_info.cipherText.size()));
        change_pwd.bind(2,new_pass_info.iv.data(),static_cast<int>(new_pass_info.iv.size()));
        change_pwd.bind(3,old_info.pwdData.cipherText.data(),static_cast<int>(old_info.pwdData.cipherText.size()));

        change_pwd.exec();

        return true;
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
}


bool db_manager::delete_password(PasswordStore info,SQLite::Database& db){
    try
    {
        SQLite::Statement del_query(
            db,
            "DELETE FROM password_store WHERE id = ?"
        );

        del_query.bind(1,info.id);
        del_query.exec();

        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
}


PasswordStore db_manager::view_password(string site,SQLite::Database& db){
    try
    {
        SQLite::Statement get_pass(
            db,
            "SELECT id,site,encrypted_pwd,iv,created_at,last_modified FROM password_store WHERE site = ?"
        );

        get_pass.bind(1,site);
        PasswordStore password;

        if (get_pass.executeStep())
        {
            password.id = get_pass.getColumn(0).getInt();
            password.site_url = get_pass.getColumn(1).getString();

            const void* data_blob = get_pass.getColumn(2).getBlob();
            int data_size = get_pass.getColumn(2).getBytes();
            password.pwdData.cipherText = read_blob(data_blob,data_size);

            const void* iv_blob = get_pass.getColumn(3).getBlob();
            int iv_size = get_pass.getColumn(3).getBytes();
            password.pwdData.iv = read_blob(iv_blob,iv_size);

            password.created_at = get_pass.getColumn(4).getString();
            password.last_modified = get_pass.getColumn(5).getString();

            return password;
        }
        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}


std::vector<PasswordStore> db_manager:: view_passwords(SQLite::Database& db){
    try
    {
        SQLite::Statement pwds_query(
            db,
            "SELECT id,site,encrypted_pwd,iv,created_at,last_modified FROM password_store "
        );

        vector<PasswordStore> passwords;

        while (pwds_query.executeStep())
        {
            PasswordStore password;

            password.id = pwds_query.getColumn(0).getInt();
            password.site_url = pwds_query.getColumn(1).getString();

            const void* data_blob = pwds_query.getColumn(2).getBlob();
            int data_size = pwds_query.getColumn(2).getBytes();
            password.pwdData.cipherText = read_blob(data_blob,data_size);

            const void* iv_blob = pwds_query.getColumn(3).getBlob();
            int iv_size = pwds_query.getColumn(3).getBytes();
            password.pwdData.iv = read_blob(iv_blob,iv_size);

            password.created_at = pwds_query.getColumn(4).getString();
            password.last_modified = pwds_query.getColumn(5).getString();

            passwords.push_back(password);
        }
        
        return passwords;

    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

}

vector<unsigned char> read_blob(const void* blob,int size){
    vector<unsigned char> iv(
        static_cast<const unsigned char*>(blob),
        static_cast<const unsigned char*>(blob) + size
    );

    return iv;
}