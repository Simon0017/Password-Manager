#include<sodium.h>
#include<vector>
#include <fstream>
#include<iostream>

#include<cryptopp/aes.h>
#include<cryptopp/gcm.h>
#include<cryptopp/osrng.h>
#include<cryptopp/filters.h>

#include"crypto_manager.hpp"

using namespace CryptoPP;

string PasswordHandlers::hash_password(string password){
    char hashed_password[crypto_pwhash_STRBYTES];

    if (crypto_pwhash_str(
        hashed_password,
        password.c_str(),
        password.length(),
        crypto_pwhash_OPSLIMIT_INTERACTIVE,
        crypto_pwhash_MEMLIMIT_INTERACTIVE

    ) != 0)
    {
        return hashed_password;
    }
    
}

bool PasswordHandlers::verify_password(string pass_hash,string password){
    return crypto_pwhash_str_verify(
        pass_hash.c_str(),
        password.c_str(),
        password.length()
    ) == 0;
}


constexpr size_t KEY_SIZE = 32;
constexpr size_t IV_SIZE = 12;

vector<unsigned char> DataEncryptionDecrytption::generate_key(){
    vector<unsigned char> key(32,0x42);
    randombytes_buf(key.data(), key.size());
    
    return key;
}


EncryptedData DataEncryptionDecrytption::encrypt(string& plaintxt,vector<unsigned char>& key){
    if (key.size() != KEY_SIZE)
    {
        throw runtime_error("Invalid key size");
    }

    AutoSeededRandomPool rng;
    EncryptedData result_data;
    result_data.iv.resize(IV_SIZE);
    rng.GenerateBlock(result_data.iv.data(),result_data.iv.size());

    GCM<AES>::Encryption enc;
    enc.SetKeyWithIV(
        key.data(),
        key.size(),
        result_data.iv.data(),
        result_data.iv.size()
    );

    StringSource ss(
        plaintxt,
        true,
        new AuthenticatedEncryptionFilter(
            enc,
            new VectorSink(result_data.cipherText)
        )
    );

    return result_data;
    
}

string DataEncryptionDecrytption::decrypt(EncryptedData& data,vector<unsigned char>& key){
    if (key.size() != KEY_SIZE)
    {
        throw runtime_error("Invalid key size");
    }

    GCM<AES>::Decryption dec;
    dec.SetKeyWithIV(
        key.data(),
        key.size(),
        data.iv.data(),
        data.iv.size()
    );

    string originaltxt;

    AuthenticatedDecryptionFilter df(
        dec,
        new StringSink(originaltxt)
    );

    StringSource ss(
        data.cipherText.data(),
        data.cipherText.size(),
        true,
        new Redirector(df)
    );

    if (!df.GetLastResult())
    {
        throw runtime_error("Data decryption failed");
    }
    
    return originaltxt;
}

void DataEncryptionDecrytption::store_key(vector<unsigned char> key){
    
    ofstream key_file("data/key.txt");

    if (key_file.is_open())
    {
        for (auto& byte : key)
        {
            key_file << byte;
        }

        key_file.close();
        
    }else
    {
        cerr << "Error opening the file\n"; 
    }
    
    
};

vector<unsigned char> DataEncryptionDecrytption::load_key(){
    vector<unsigned char> key;
    ifstream key_file("data/key.txt");

    if (key_file.is_open())
    {
        unsigned char byte;
        while (key_file >> byte)
        {
            key.push_back(byte);
        }

        key_file.close();
        
    }else
    {
        cerr << "Error opening the file\n"; 
    }

    return key;
}