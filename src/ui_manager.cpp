#include "ui_manager.h"
#include "message_box.h"

UIManager::UIManager()
    : m_currentState(AppState::LOGIN),
      m_showRegistration(false),
      m_showLogin(true),
      m_showMainWindow(false),
      m_showSavePassword(false),
      m_showEditPassword(false),
      m_showUserManagement(false)
{
}

UIManager::~UIManager()
{
}

void UIManager::Render()
{
    // Render windows based on current state
    if (m_showRegistration)
    {
        m_registrationWindow.Render(&m_showRegistration);
    }
    
    if (m_showLogin)
    {
        m_loginWindow.Render(&m_showLogin);
    }
    
    if (m_showMainWindow)
    {
        m_mainWindow.Render(&m_showMainWindow);
        
        // Check if sub-windows should be opened
        if (m_mainWindow.ShouldOpenSaveWindow())
        {
            m_showSavePassword = true;
            m_mainWindow.ResetFlags();
        }
        
        if (m_mainWindow.ShouldOpenEditWindow())
        {
            const PasswordStore* selectedPwd = m_mainWindow.GetSelectedPassword();
            if (selectedPwd)
            {
                m_editPasswordWindow.SetPasswordStore(*selectedPwd);
                m_showEditPassword = true;
            }
            m_mainWindow.ResetFlags();
        }
        
        if (m_mainWindow.ShouldOpenUserManagement())
        {
            // TODO: Set current user data
            // User currentUser = GetCurrentLoggedInUser();
            // m_userManagementWindow.SetUser(currentUser);
            
            m_showUserManagement = true;
            m_mainWindow.ResetFlags();
        }
    }
    
    if (m_showSavePassword)
    {
        m_savePasswordWindow.Render(&m_showSavePassword);
    }
    
    if (m_showEditPassword)
    {
        m_editPasswordWindow.Render(&m_showEditPassword);
    }
    
    if (m_showUserManagement)
    {
        m_userManagementWindow.Render(&m_showUserManagement);
    }
    
    // Always render message box if open
    MessageBox::Render();
}

void UIManager::SetAppState(AppState state)
{
    m_currentState = state;
    
    // Reset all window states
    m_showRegistration = false;
    m_showLogin = false;
    m_showMainWindow = false;
    m_showSavePassword = false;
    m_showEditPassword = false;
    m_showUserManagement = false;
    
    // Set appropriate window based on state
    switch (state)
    {
        case AppState::REGISTRATION:
            m_showRegistration = true;
            break;
        case AppState::LOGIN:
            m_showLogin = true;
            break;
        case AppState::MAIN_DASHBOARD:
            m_showMainWindow = true;
            // TODO: Load password stores from database
            // std::vector<PasswordStore> stores = LoadUserPasswordStores();
            // m_mainWindow.SetPasswordStores(stores);
            break;
    }
}