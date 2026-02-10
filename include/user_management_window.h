#pragma once
#include "models.hpp"

class UserManagementWindow
{
public:
    UserManagementWindow();
    void Render(bool* p_open);
    void SetUser(const User& user);
    void Reset();

private:
    User m_currentUser;
    char m_newPassword[128];
    char m_confirmPassword[128];
    bool m_showPassword;
    bool m_showConfirmPassword;
    bool m_hasUser;
};