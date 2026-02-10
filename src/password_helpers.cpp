#include<string>
#include<regex>
#include<random>
#include<algorithm>
#include<vector>

#include"models.hpp"
#include"password_helpers.hpp"

using namespace std;

bool has_min_length(const string& password,size_t min = 8){
    return password.size() >= min;
}

bool has_lowercase(const string& password){
    static const regex r("[a-z]");
    return regex_search(password,r);
}

bool has_uppercase(const string& password){
    static const regex r("[A-Z]");
    return regex_search(password,r);
}

bool has_digit(const string& password) {
    static const regex r("[0-9]");
    return regex_search(password, r);
}

bool has_special_char(const string& password) {
    static const regex r("[!@#$%^&*\\-?]");
    return regex_search(password, r);
}

PasswordVerifier verify_password(const string& password){
    PasswordVerifier result;

    if (!has_min_length(password))
    {
        result.is_valid = false;
        result.error_message.push_back("Password must have a minimum if 8 characters");
    }

    if (!has_lowercase(password))
    {
        result.is_valid = false;
        result.error_message.push_back("Password must contain a lowercase letter");

    }

    if (!has_uppercase(password))
    {
        result.is_valid = false;
        result.error_message.push_back("Password must contain an uppercase letter");
    }

    if (!has_special_char(password))
    {
        result.is_valid = false;
        result.error_message.push_back("Password must have a special character");
    }
    
    if (!has_digit(password))
    {
        result.is_valid = false;
        result.error_message.push_back("Password must have at least one digit");
    }
    
    return result;

}

char random_char(const string& charset) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, (charset.size()) - 1);
    return charset[dist(gen)];
}

string generate_password(size_t length){
    const string lower   = "abcdefghijklmnopqrstuvwxyz";
    const string upper   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string digit   = "0123456789";
    const string special = "!@#$%^&*-?";

    if ( length < 8)
    {
       throw invalid_argument("Password length must be atleast 8");
    }

    string password;
    password.reserve(length);

    password.push_back(random_char(lower));
    password.push_back(random_char(upper));
    password.push_back(random_char(digit));
    password.push_back(random_char(special));

    string all = lower + upper + digit + special;

    while (password.size() < length)
    {
        password.push_back(random_char(all));
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::shuffle(password.begin(), password.end(), gen);

    return password;

}

bool passes_password_test(const string& password){
    static const regex password_regex(R"(^(?=.*[a-z])(?=.*[A-Z])(?=.*[0-9])(?=.*[!@#$%^&*-?]).{8,}$)");
    return regex_match(password,password_regex);
}