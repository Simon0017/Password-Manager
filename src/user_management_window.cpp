#include "user_management_window.h"
#include "message_box.h"
#include "imgui.h"
#include <cstring>

UserManagementWindow::UserManagementWindow()
    : m_showPassword(false), m_showConfirmPassword(false), m_hasUser(false)
{
    memset(m_newPassword, 0, sizeof(m_newPassword));
    memset(m_confirmPassword, 0, sizeof(m_confirmPassword));
}

void UserManagementWindow::Render(bool* p_open)
{
    if (!m_hasUser)
    {
        *p_open = false;
        return;
    }
    
    ImGui::SetNextWindowSize(ImVec2(550, 450), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    
    if (ImGui::Begin("User Management", p_open, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
        
        ImGui::Text("Account Settings");
        ImGui::Separator();
        ImGui::Spacing();
        
        // Display current user info
        ImGui::Text("Current Account Information");
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f), "Username: %s", m_currentUser.username.c_str());
        ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f), "Email: %s", m_currentUser.email.c_str());
        ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f), "Address: %s", m_currentUser.address.c_str());
        
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        // Change password section
        ImGui::Text("Change Password");
        ImGui::Spacing();
        
        ImGui::Text("New Password:");
        ImGui::SetNextItemWidth(-1);
        if (m_showPassword)
            ImGui::InputText("##newpassword", m_newPassword, IM_ARRAYSIZE(m_newPassword));
        else
            ImGui::InputText("##newpassword", m_newPassword, IM_ARRAYSIZE(m_newPassword), ImGuiInputTextFlags_Password);
        ImGui::SameLine();
        ImGui::Checkbox("Show##pwd", &m_showPassword);
        ImGui::Spacing();
        
        ImGui::Text("Confirm Password:");
        ImGui::SetNextItemWidth(-1);
        if (m_showConfirmPassword)
            ImGui::InputText("##confirmpassword", m_confirmPassword, IM_ARRAYSIZE(m_confirmPassword));
        else
            ImGui::InputText("##confirmpassword", m_confirmPassword, IM_ARRAYSIZE(m_confirmPassword), ImGuiInputTextFlags_Password);
        ImGui::SameLine();
        ImGui::Checkbox("Show##cpwd", &m_showConfirmPassword);
        
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        float buttonWidth = 140.0f;
        float spacing = 15.0f;
        float totalWidth = (buttonWidth * 3) + (spacing * 2);
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);
        
        if (ImGui::Button("Update Password", ImVec2(buttonWidth, 40)))
        {
            if (strlen(m_newPassword) == 0)
            {
                MessageBox::Show("Error", "Password cannot be empty", MessageBox::ERROR_MSG);
            }
            else if (strcmp(m_newPassword, m_confirmPassword) != 0)
            {
                MessageBox::Show("Error", "Passwords do not match", MessageBox::ERROR_MSG);
            }
            else
            {
                // TODO: Insert your update password logic here
                // Hash the new password
                // Update m_currentUser.password_hash in database
                // Optionally regenerate encryption key
                // On success:
                // MessageBox::Show("Success", "Password updated successfully!", MessageBox::SUCCESS);
                // Clear password fields
                // memset(m_newPassword, 0, sizeof(m_newPassword));
                // memset(m_confirmPassword, 0, sizeof(m_confirmPassword));
                
                MessageBox::Show("Info", "Update password logic not implemented yet", MessageBox::INFO);
            }
        }
        
        ImGui::SameLine(0, spacing);
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
        
        if (ImGui::Button("Delete Account", ImVec2(buttonWidth, 40)))
        {
            // TODO: Insert your delete account logic here
            // Show confirmation dialog
            // Delete all password entries associated with user
            // Delete user account from database
            // Clear all session data
            // On success:
            // MessageBox::Show("Info", "Account deleted. Redirecting to registration...", MessageBox::INFO);
            // Logout and redirect to registration/login
            // Reset();
            // *p_open = false;
            
            MessageBox::Show("Warning", "Delete account logic not implemented yet", MessageBox::WARNING);
        }
        
        ImGui::PopStyleColor(3);
        
        ImGui::SameLine(0, spacing);
        
        if (ImGui::Button("Close", ImVec2(buttonWidth, 40)))
        {
            Reset();
            *p_open = false;
        }
        
        ImGui::PopStyleVar();
    }
    ImGui::End();
}

void UserManagementWindow::SetUser(const User& user)
{
    m_currentUser = user;
    m_hasUser = true;
}

void UserManagementWindow::Reset()
{
    memset(m_newPassword, 0, sizeof(m_newPassword));
    memset(m_confirmPassword, 0, sizeof(m_confirmPassword));
    m_showPassword = false;
    m_showConfirmPassword = false;
}