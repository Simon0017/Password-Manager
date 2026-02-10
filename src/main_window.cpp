#include "main_window.h"
#include "message_box.h"
#include "imgui.h"
#include <cstring>
#include <algorithm>

MainWindow::MainWindow()
    : m_selectedPasswordId(-1), m_showPassword(false),
      m_openSaveWindow(false), m_openEditWindow(false), m_openUserManagement(false)
{
    memset(m_searchQuery, 0, sizeof(m_searchQuery));
}

void MainWindow::Render(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(1200, 700), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    
    if (ImGui::Begin("Password Manager Dashboard", p_open, ImGuiWindowFlags_MenuBar))
    {
        // Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("Account"))
            {
                if (ImGui::MenuItem("User Settings"))
                {
                    m_openUserManagement = true;
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Logout"))
                {
                    // TODO: Insert logout logic here
                    // Clear session data
                    // Clear sensitive data from memory
                    // Return to login screen
                    
                    MessageBox::Show("Info", "Logout logic not implemented yet", MessageBox::INFO);
                }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        // Split window layout (1:3 ratio)
        float leftPanelWidth = ImGui::GetContentRegionAvail().x * 0.25f;
        float rightPanelWidth = ImGui::GetContentRegionAvail().x * 0.75f - 10.0f;
        
        // Left Panel - Password List
        ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), true);
        {
            ImGui::Text("Your Passwords");
            ImGui::Separator();
            ImGui::Spacing();
            
            // Search field
            ImGui::SetNextItemWidth(-1);
            ImGui::InputTextWithHint("##search", "Search site or email...", m_searchQuery, IM_ARRAYSIZE(m_searchQuery));
            
            ImGui::Spacing();
            
            // Add new password button
            if (ImGui::Button("+ Add Password", ImVec2(-1, 35)))
            {
                m_openSaveWindow = true;
            }
            
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();
            
            // Password list
            ImGui::BeginChild("PasswordList", ImVec2(0, 0), false);
            {
                std::string searchStr = m_searchQuery;
                std::transform(searchStr.begin(), searchStr.end(), searchStr.begin(), ::tolower);
                
                for (const auto& pwd : m_passwordStores)
                {
                    std::string siteUrl = pwd.site_url;
                    std::transform(siteUrl.begin(), siteUrl.end(), siteUrl.begin(), ::tolower);
                    
                    // Simple search filter
                    if (searchStr.empty() || siteUrl.find(searchStr) != std::string::npos)
                    {
                        bool isSelected = (pwd.id == m_selectedPasswordId);
                        
                        if (ImGui::Selectable(pwd.site_url.c_str(), isSelected, 0, ImVec2(0, 30)))
                        {
                            m_selectedPasswordId = pwd.id;
                            
                            // TODO: When password is selected, decrypt it for display
                            // Use user's encryption key to decrypt pwd.pwdData
                            // Store decrypted password temporarily for display
                        }
                        
                        if (isSelected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                }
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();
        
        ImGui::SameLine();
        
        // Right Panel - Password Details
        ImGui::BeginChild("RightPanel", ImVec2(rightPanelWidth, 0), true);
        {
            const PasswordStore* selectedPwd = GetSelectedPassword();
            
            if (selectedPwd)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
                
                ImGui::Text("Password Details");
                ImGui::Separator();
                ImGui::Spacing();
                ImGui::Spacing();
                
                // Site URL
                ImGui::Text("Site URL:");
                ImGui::Spacing();
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.9f, 1.0f, 1.0f));
                ImGui::InputText("##siteurl", const_cast<char*>(selectedPwd->site_url.c_str()), 
                                selectedPwd->site_url.length() + 1, ImGuiInputTextFlags_ReadOnly);
                ImGui::PopStyleColor(2);
                
                ImGui::Spacing();
                ImGui::Spacing();
                
                // Password with toggle visibility
                ImGui::Text("Password:");
                ImGui::Spacing();
                
                // TODO: Display decrypted password here
                // For now showing placeholder
                static char displayPassword[256] = "**********";
                
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.2f, 1.0f));
                if (m_showPassword)
                {
                    // TODO: Show actual decrypted password
                    // strcpy(displayPassword, decryptedPassword.c_str());
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.5f, 1.0f));
                    ImGui::InputText("##password", displayPassword, IM_ARRAYSIZE(displayPassword), ImGuiInputTextFlags_ReadOnly);
                    ImGui::PopStyleColor();
                }
                else
                {
                    strcpy(displayPassword, "**********");
                    ImGui::InputText("##password", displayPassword, IM_ARRAYSIZE(displayPassword), ImGuiInputTextFlags_ReadOnly | ImGuiInputTextFlags_Password);
                }
                ImGui::PopStyleColor();
                
                ImGui::SameLine();
                if (ImGui::Button(m_showPassword ? "Hide" : "Show", ImVec2(80, 0)))
                {
                    m_showPassword = !m_showPassword;
                }
                
                ImGui::SameLine();
                if (ImGui::Button("Copy", ImVec2(80, 0)))
                {
                    // TODO: Copy decrypted password to clipboard
                    // ImGui::SetClipboardText(decryptedPassword.c_str());
                    
                    MessageBox::Show("Info", "Password copied to clipboard!", MessageBox::SUCCESS);
                }
                
                ImGui::Spacing();
                ImGui::Spacing();
                
                // Date Created
                ImGui::Text("Date Created:");
                ImGui::Spacing();
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
                ImGui::InputText("##created", const_cast<char*>(selectedPwd->created_at.c_str()), 
                                selectedPwd->created_at.length() + 1, ImGuiInputTextFlags_ReadOnly);
                ImGui::PopStyleColor(2);
                
                ImGui::Spacing();
                ImGui::Spacing();
                
                // Last Modified
                ImGui::Text("Last Modified:");
                ImGui::Spacing();
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.15f, 0.15f, 0.2f, 1.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.7f, 0.7f, 0.7f, 1.0f));
                ImGui::InputText("##modified", const_cast<char*>(selectedPwd->last_modified.c_str()), 
                                selectedPwd->last_modified.length() + 1, ImGuiInputTextFlags_ReadOnly);
                ImGui::PopStyleColor(2);
                
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();
                
                // Edit button
                if (ImGui::Button("Edit Password", ImVec2(150, 40)))
                {
                    m_openEditWindow = true;
                }
                
                ImGui::PopStyleVar();
            }
            else
            {
                ImGui::SetCursorPosY(ImGui::GetWindowHeight() * 0.4f);
                ImGui::SetCursorPosX((ImGui::GetWindowWidth() - 300) * 0.5f);
                ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Select a password to view details");
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void MainWindow::SetPasswordStores(const std::vector<PasswordStore>& stores)
{
    m_passwordStores = stores;
}

void MainWindow::ResetFlags()
{
    m_openSaveWindow = false;
    m_openEditWindow = false;
    m_openUserManagement = false;
}

const PasswordStore* MainWindow::GetSelectedPassword() const
{
    for (const auto& pwd : m_passwordStores)
    {
        if (pwd.id == m_selectedPasswordId)
            return &pwd;
    }
    return nullptr;
}