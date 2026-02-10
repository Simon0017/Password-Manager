#pragma once
#include "models.hpp"
#include <vector>

class MainWindow
{
public:
    MainWindow();
    void Render(bool* p_open);
    void SetPasswordStores(const std::vector<PasswordStore>& stores);
    bool ShouldOpenSaveWindow() const { return m_openSaveWindow; }
    bool ShouldOpenEditWindow() const { return m_openEditWindow; }
    bool ShouldOpenUserManagement() const { return m_openUserManagement; }
    void ResetFlags();
    int GetSelectedPasswordId() const { return m_selectedPasswordId; }
    const PasswordStore* GetSelectedPassword() const;

private:
    std::vector<PasswordStore> m_passwordStores;
    char m_searchQuery[256];
    int m_selectedPasswordId;
    bool m_showPassword;
    bool m_openSaveWindow;
    bool m_openEditWindow;
    bool m_openUserManagement;
};