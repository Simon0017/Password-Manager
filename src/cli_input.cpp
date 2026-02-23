#include"cli_input.hpp"
#include<fmt/core.h>
#include<iostream>
#include<sstream>
#include"db_manager.hpp"
#include"models.hpp"
#include"crypto_manager.hpp"


#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/VariadicBind.h>

using namespace std;
using namespace fmt;

InputManager::Action InputManager::get_user_action(string input)
{
    if(input == "login")
        return Action::LOGIN;
    else if(input == "register")
        return Action::REGISTER;
    else if(input == "get_password")
        return Action::GET_PASSWORD;
    else if(input == "insert_password")
        return Action::INSERT_PASSWORD;
    else if(input == "change_password")
        return Action::CHANGE_PASSWORD;
    else if(input == "delete_password")
        return Action::DELETE_PASSWORD;
    else if(input == "view_passwords")
        return Action::VIEW_PASSWORDS;
    else if(input == "change_user_password")
        return Action::CHANGE_USER_PASSWORD;
    else if(input == "delete_user")
        return Action::DELETE_USER;
    else if (input == "back")
        return Action::BACK;
    else if(input == "exit")
        return Action::EXIT;
    else return Action::EXIT;
    
    throw printf("Invalid input: %s", input.c_str());
}


// handle the actions
void InputManager::handle_user_action(Action action, SQLite::Database& db){
    switch(action){
        case Action::LOGIN:
            login(db);
            break;
        case Action::REGISTER:
            register_user(db);
            break;
        case Action::GET_PASSWORD:
            get_password(db);
            break;
        case Action::INSERT_PASSWORD:
            insert_password(db);
            break;
        case Action::CHANGE_PASSWORD:
            change_password(db);
            break;
        case Action::DELETE_PASSWORD:
            delete_password(db);
            break;
        case Action::VIEW_PASSWORDS:
            view_passwords(db);
            break;
        case Action::CHANGE_USER_PASSWORD:
            change_user_password(db);
            break;
        case Action::DELETE_USER:
            delete_user(db);
            break;
        case Action::EXIT:
            exit_program();
            break;
        case Action::BACK:
            back_to_menu();
            break;
        default:
            throw std::runtime_error("Invalid action");;
    }
}

void login(SQLite::Database& db){
    string username = get_user_input("Enter username: ");
    string password = get_user_input("Enter password: ");

    db_manager DBmanager;
    User user_info = DBmanager.get_user(username, db);

    PasswordHandlers pwd;
    if (pwd.verify_password(user_info.password_hash,password))
    {
        print("User: {} has been authenticated,proceed...",user_info.username);
    }else
    {
        print("Wrong credentials.Try again\n");
    }
    
}

void register_user(SQLite::Database& db){
    string username = get_user_input("Enter username: ");
    string password = get_user_input("Enter password: ");
    string email = get_user_input("Enter email: ");
    string addr = get_user_input("Enter address: ");
    vector<unsigned char> key;

    db_manager DBManager;
    PasswordHandlers pwd;
    DataEncryptionDecrytption cryptoManager;


    string pwd_hash = pwd.hash_password(password);
    
    User user_info;

    user_info.username = username;
    user_info.email = email;
    user_info.address = addr;
    user_info.password_hash = pwd_hash;

   try {
        DBManager.register_user(user_info, db);
        key = cryptoManager.generate_key();
        cryptoManager.store_key(key);

        print("User {} has been registered", user_info.username);
    } catch (const std::exception& e) {
        print("Failed to register user {}: {}\n", username, e.what());
    }
}

void get_password(SQLite::Database& db){
    string url = get_user_input("Enter site url: ");

    db_manager DBManager;
    PasswordStore pwd_info = DBManager.view_password(url, db);
    DataEncryptionDecrytption cryptoManager;
    vector<unsigned char> key = cryptoManager.load_key();
    string decrypted_pwd = cryptoManager.decrypt(pwd_info.pwdData, key);
    print("Password for {}\tis: {}",url,decrypted_pwd);
}

void insert_password(SQLite::Database& db){
    string url = get_user_input("Enter site url: ");
    string password = get_user_input("Enter password: ");
    string username = get_user_input("Enter username: ");

    DataEncryptionDecrytption cryptoManager;
    vector<unsigned char> key = cryptoManager.load_key();
    EncryptedData encrypted_pwd = cryptoManager.encrypt(password, key);

    PasswordStore pwd_info{
        1,
        url,
        encrypted_pwd,
        "2024-06-01",
        "2024-06-01"
    };

    db_manager DBManager;
    User user_info = DBManager.get_user(username, db); // password not checked here, should be checked in login function before allowing user to insert password
    if (DBManager.insert_password(pwd_info, user_info, db))
    {
        print("Password for {} has been inserted",url);
    }else
    {
        print("Failed to insert password for {}",url);
    }

}

void change_password(SQLite::Database& db){
    string url = get_user_input("Enter site url: ");
    string new_password = get_user_input("Enter new password: ");

    DataEncryptionDecrytption cryptoManager;
    vector<unsigned char> key = cryptoManager.load_key();
    EncryptedData encrypted_pwd = cryptoManager.encrypt(new_password, key);

    db_manager DBManager;
    PasswordStore old_pwd_info = DBManager.view_password(url, db);
    if (DBManager.change_password(old_pwd_info, encrypted_pwd, db))
    {
        print("Password for {} has been changed",url);
    }else
    {
        print("Failed to change password for {}",url);
    }
}

void delete_password(SQLite::Database& db){
    string url = get_user_input("Enter site url: ");

    // confirm identity and confirmation b4 deletion
    string username = get_user_input("Enter username: ");
    string password = get_user_input("Enter password: ");

    

    db_manager DBManager;
    User user_info = DBManager.get_user(username, db);
    PasswordHandlers pwd;
    if (!pwd.verify_password(user_info.password_hash,password))
    {
        print("Wrong credentials. Cannot delete password.\n");
        return;
    }

    string confirmation = get_user_input("Are you sure you want to delete the password for " + url + "? (yes/no): ");
    if (confirmation != "yes")
    {
        print("Password deletion cancelled.");
        return;
    }

    PasswordStore pwd_info = DBManager.view_password(url, db);
    if (DBManager.delete_password(pwd_info, db))
    {
        print("Password for {} has been deleted",url);
    }else
    {
        print("Failed to delete password for {}",url);
    }
}

void view_passwords(SQLite::Database& db){
    /* To view all the user's password in a table*/
    string username = get_user_input("Enter username: ");
    string password = get_user_input("Enter password: ");
  
    db_manager DBManager;
    User user_info = DBManager.get_user(username, db);
    PasswordHandlers pwd;
    if (!pwd.verify_password(user_info.password_hash,password))
    {
        print("Wrong credentials. Cannot view passwords.\n");
        return;
    }

    vector<PasswordStore> passwords = DBManager.view_passwords(db);
    print("Passwords for user {}:", username);
    for (auto& pwd_info : passwords)
    {
        DataEncryptionDecrytption cryptoManager;
        vector<unsigned char> key = cryptoManager.load_key();
        string decrypted_pwd = cryptoManager.decrypt(pwd_info.pwdData, key);
        print("Site: {}, Password: {}, Created At: {}, Last Modified: {}",
                pwd_info.site_url, decrypted_pwd, pwd_info.created_at, pwd_info.last_modified);
    }  
}

void change_user_password(SQLite::Database& db){
    string username = get_user_input("Enter username: ");
    string old_password = get_user_input("Enter Old password: ");
  
    db_manager DBManager;
    User user_info = DBManager.get_user(username, db);
    PasswordHandlers pwd;
    if (!pwd.verify_password(user_info.password_hash,old_password))
    {
        print("Wrong credentials. Cannot change user password.\n");
        return;
    }

    string new_password = get_user_input("Enter New Password");
    string new_pwd_hash = pwd.hash_password(new_password);

    bool success = DBManager.change_user_password(user_info.username,new_pwd_hash,db);
}

void delete_user(SQLite::Database& db){
    string username = get_user_input("Enter username: ");
    string old_password = get_user_input("Enter Old password: ");
  
    db_manager DBManager;
    User user_info = DBManager.get_user(username, db);
    PasswordHandlers pwd;
    if (!pwd.verify_password(user_info.password_hash,old_password))
    {
        print("Wrong credentials. Cannot delete user.\n");
        return;
    }

   bool success =  DBManager.delete_user(user_info.username,db);
}

void exit_program(){
    
}

void back_to_menu(){
    display_menu();
}


void display_menu(){
    print("Welcome to the Password Manager CLI!\n");
    print("Please select an option:\n");
    print("1. Login (type 'login')\n");
    print("2. Register (type 'register')\n");
    print("3. Get Password (type 'get_password')\n");
    print("4. Insert Password (type 'insert_password')\n");
    print("5. Change Password (type 'change_password')\n");
    print("6. Delete Password (type 'delete_password')\n");
    print("7. View Passwords (type 'view_passwords')\n");
    print("8. Change User Password (type 'change_user_password')\n");
    print("9. Delete User (type 'delete_user')\n");
    print("10. Back to Menu (type 'back')\n");
    print("11. Exit (type 'exit')\n");
}

string get_user_input(string prompt){
    print(prompt);

    string input_ss;
    getline(cin,input_ss);

    return input_ss;
}