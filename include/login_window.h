#pragma once
#include "models.hpp"

class LoginWindow
{
public:
    LoginWindow();
    void Render(bool* p_open);
    void Reset();

private:
    char m_username[128];
    char m_password[128];
    bool m_showPassword;
};