#pragma once
#include "models.hpp"

class EditPasswordWindow
{
public:
    EditPasswordWindow();
    void Render(bool* p_open);
    void SetPasswordStore(const PasswordStore& store);
    void Reset();

private:
    PasswordStore m_currentStore;
    char m_newPassword[256];
    bool m_showPassword;
    bool m_hasData;
};