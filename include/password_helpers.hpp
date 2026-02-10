#pragma once
#include<string>

#include"models.hpp"

using namespace std;

PasswordVerifier verify_password(const string& password);
string generate_password(size_t length =12 );
bool passes_password_test(const string& password);