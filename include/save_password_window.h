#pragma once
#include "models.hpp"

class SavePasswordWindow
{
public:
    SavePasswordWindow();
    void Render(bool* p_open);
    void Reset();

private:
    char m_siteUrl[512];
    char m_password[256];
    bool m_showPassword;
};