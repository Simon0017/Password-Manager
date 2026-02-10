#pragma once
#include<iostream>
#include<vector>

using namespace std;

struct User
{   
    int id;
    string username;
    string email; // for recovery
    string address;
    string password_hash;
    vector<unsigned char> key;
};

struct EncryptedData
{
    vector<unsigned char> iv;
    vector<unsigned char> cipherText;
};

struct PasswordStore
{
    int id;
    string site_url;
    EncryptedData pwdData;
    string created_at;
    string last_modified;

};

struct PasswordVerifier
{
    bool is_valid;
    vector<string> error_message;
};
