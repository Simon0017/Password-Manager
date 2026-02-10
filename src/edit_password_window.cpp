#include "edit_password_window.h"
#include "message_box.h"
#include "imgui.h"
#include <cstring>

EditPasswordWindow::EditPasswordWindow()
    : m_showPassword(false), m_hasData(false)
{
    memset(m_newPassword, 0, sizeof(m_newPassword));
}

void EditPasswordWindow::Render(bool* p_open)
{
    if (!m_hasData)
    {
        *p_open = false;
        return;
    }
    
    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    
    if (ImGui::Begin("Edit Password", p_open, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
        
        ImGui::Text("Edit Password Entry");
        ImGui::Separator();
        ImGui::Spacing();
        
        ImGui::Text("Site URL:");
        ImGui::TextColored(ImVec4(0.6f, 0.8f, 1.0f, 1.0f), "%s", m_currentStore.site_url.c_str());
        ImGui::Spacing();
        ImGui::Spacing();
        
        ImGui::Text("New Password:");
        ImGui::SetNextItemWidth(-1);
        if (m_showPassword)
            ImGui::InputText("##newpassword", m_newPassword, IM_ARRAYSIZE(m_newPassword));
        else
            ImGui::InputText("##newpassword", m_newPassword, IM_ARRAYSIZE(m_newPassword), ImGuiInputTextFlags_Password);
        ImGui::SameLine();
        ImGui::Checkbox("Show", &m_showPassword);
        
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        float buttonWidth = 120.0f;
        float spacing = 15.0f;
        float totalWidth = (buttonWidth * 3) + (spacing * 2);
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);
        
        if (ImGui::Button("Update", ImVec2(buttonWidth, 40)))
        {
            if (strlen(m_newPassword) == 0)
            {
                MessageBox::Show("Error", "Password cannot be empty", MessageBox::ERROR_MSG);
            }
            else
            {
                // TODO: Insert your update password logic here
                // Encrypt m_newPassword using user's encryption key
                // Update m_currentStore.pwdData with new encrypted data
                // Update m_currentStore.last_modified to current timestamp
                // Save changes to database using m_currentStore.id
                // On success:
                // MessageBox::Show("Success", "Password updated successfully!", MessageBox::SUCCESS);
                // Reset();
                // *p_open = false;
                
                MessageBox::Show("Info", "Update password logic not implemented yet", MessageBox::INFO);
            }
        }
        
        ImGui::SameLine(0, spacing);
        
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
        
        if (ImGui::Button("Delete", ImVec2(buttonWidth, 40)))
        {
            // TODO: Insert your delete password logic here
            // Confirm deletion with user
            // Delete password entry from database using m_currentStore.id
            // On success:
            // MessageBox::Show("Success", "Password deleted successfully!", MessageBox::SUCCESS);
            // Reset();
            // *p_open = false;
            
            MessageBox::Show("Warning", "Delete password logic not implemented yet", MessageBox::WARNING);
        }
        
        ImGui::PopStyleColor(3);
        
        ImGui::SameLine(0, spacing);
        
        if (ImGui::Button("Cancel", ImVec2(buttonWidth, 40)))
        {
            Reset();
            *p_open = false;
        }
        
        ImGui::PopStyleVar();
    }
    ImGui::End();
}

void EditPasswordWindow::SetPasswordStore(const PasswordStore& store)
{
    m_currentStore = store;
    m_hasData = true;
    memset(m_newPassword, 0, sizeof(m_newPassword));
}

void EditPasswordWindow::Reset()
{
    memset(m_newPassword, 0, sizeof(m_newPassword));
    m_showPassword = false;
    m_hasData = false;
}