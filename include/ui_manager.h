#pragma once
#include "registration_window.h"
#include "login_window.h"
#include "main_window.h"
#include "save_password_window.h"
#include "edit_password_window.h"
#include "user_management_window.h"

class UIManager
{
public:
    UIManager();
    ~UIManager();
    
    void Render();
    
    enum class AppState
    {
        REGISTRATION,
        LOGIN,
        MAIN_DASHBOARD
    };
    
    void SetAppState(AppState state);

private:
    AppState m_currentState;
    
    bool m_showRegistration;
    bool m_showLogin;
    bool m_showMainWindow;
    bool m_showSavePassword;
    bool m_showEditPassword;
    bool m_showUserManagement;
    
    RegistrationWindow m_registrationWindow;
    LoginWindow m_loginWindow;
    MainWindow m_mainWindow;
    SavePasswordWindow m_savePasswordWindow;
    EditPasswordWindow m_editPasswordWindow;
    UserManagementWindow m_userManagementWindow;
};