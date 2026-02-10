#pragma once

#include"models.hpp"

class RegistrationWindow
{
public:
    RegistrationWindow();
    void Render(bool* p_open);
    void Reset();

private:
    char m_username[128];
    char m_email[256];
    char m_address[512];
    char m_password[128];
    char m_confirmPassword[128];
    bool m_showPassword;
    bool m_showConfirmPassword;
};