#include "save_password_window.h"
#include "message_box.h"
#include "imgui.h"
#include <cstring>

SavePasswordWindow::SavePasswordWindow()
    : m_showPassword(false)
{
    memset(m_siteUrl, 0, sizeof(m_siteUrl));
    memset(m_password, 0, sizeof(m_password));
}

void SavePasswordWindow::Render(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 280), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    
    if (ImGui::Begin("Save New Password", p_open, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
        
        ImGui::Text("Add New Password Entry");
        ImGui::Separator();
        ImGui::Spacing();
        
        ImGui::Text("Site URL:");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##siteurl", m_siteUrl, IM_ARRAYSIZE(m_siteUrl));
        ImGui::Spacing();
        
        ImGui::Text("Password:");
        ImGui::SetNextItemWidth(-1);
        if (m_showPassword)
            ImGui::InputText("##password", m_password, IM_ARRAYSIZE(m_password));
        else
            ImGui::InputText("##password", m_password, IM_ARRAYSIZE(m_password), ImGuiInputTextFlags_Password);
        ImGui::SameLine();
        ImGui::Checkbox("Show", &m_showPassword);
        
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();
        
        float buttonWidth = 150.0f;
        float spacing = 20.0f;
        float totalWidth = (buttonWidth * 2) + spacing;
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);
        
        if (ImGui::Button("Save", ImVec2(buttonWidth, 40)))
        {
            // Validation
            if (strlen(m_siteUrl) == 0)
            {
                MessageBox::Show("Error", "Site URL cannot be empty", MessageBox::ERROR_MSG);
            }
            else if (strlen(m_password) == 0)
            {
                MessageBox::Show("Error", "Password cannot be empty", MessageBox::ERROR_MSG);
            }
            else
            {
                // TODO: Insert your save password logic here
                // PasswordStore store;
                // store.site_url = m_siteUrl;
                // Encrypt m_password using user's encryption key
                // Store encrypted data in store.pwdData
                // Set store.created_at to current timestamp
                // Set store.last_modified to current timestamp
                // Save to database
                // On success:
                // MessageBox::Show("Success", "Password saved successfully!", MessageBox::SUCCESS);
                // Reset();
                // *p_open = false;
                
                MessageBox::Show("Info", "Save password logic not implemented yet", MessageBox::INFO);
            }
        }
        
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

void SavePasswordWindow::Reset()
{
    memset(m_siteUrl, 0, sizeof(m_siteUrl));
    memset(m_password, 0, sizeof(m_password));
    m_showPassword = false;
}