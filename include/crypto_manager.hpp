#pragma once
#include<string>
#include<sodium.h>
#include<vector>

#include"models.hpp"

using namespace std;

class PasswordHandlers
{
private:
    /* data */
public:
    PasswordHandlers() {}
    ~PasswordHandlers() {}

    string hash_password(string password);
    bool verify_password(string pass_hash,string password);

};

class DataEncryptionDecrytption
{
private:
    
public:
    DataEncryptionDecrytption(){}
    ~DataEncryptionDecrytption(){}

    vector<unsigned char> generate_key();
    EncryptedData encrypt(string& plaintxt,vector<unsigned char>& key);
    string decrypt(EncryptedData& data,vector<unsigned char>& key);
};