#include "login_window.h"
#include "message_box.h"
#include "imgui.h"
#include <cstring>

LoginWindow::LoginWindow()
    : m_showPassword(false)
{
    memset(m_username, 0, sizeof(m_username));
    memset(m_password, 0, sizeof(m_password));
}

void LoginWindow::Render(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(450, 300), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    
    if (ImGui::Begin("Login", p_open, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
        
        ImGui::Text("Welcome Back!");
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Spacing();
        
        ImGui::Text("Username:");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##username", m_username, IM_ARRAYSIZE(m_username));
        ImGui::Spacing();
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
        
        if (ImGui::Button("Login", ImVec2(buttonWidth, 40)))
        {
            // Validation
            if (strlen(m_username) == 0)
            {
                MessageBox::Show("Error", "Username cannot be empty", MessageBox::ERROR_MSG);
            }
            else if (strlen(m_password) == 0)
            {
                MessageBox::Show("Error", "Password cannot be empty", MessageBox::ERROR_MSG);
            }
            else
            {
                // TODO: Insert your login logic here
                // Verify username and password against database
                // Hash the entered password and compare with stored hash
                // On success:
                // Load user data and encryption key
                // MessageBox::Show("Success", "Login successful!", MessageBox::SUCCESS);
                // Switch to main dashboard
                // Reset();
                // *p_open = false;
                // On failure:
                // MessageBox::Show("Error", "Invalid username or password", MessageBox::ERROR_MSG);
                
                MessageBox::Show("Info", "Login logic not implemented yet", MessageBox::INFO);
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

void LoginWindow::Reset()
{
    memset(m_username, 0, sizeof(m_username));
    memset(m_password, 0, sizeof(m_password));
    m_showPassword = false;
}