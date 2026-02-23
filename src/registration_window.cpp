#include "registration_window.h"
#include "message_box.h"
#include "imgui.h"
#include "models.hpp"
#include <string>
#include "crypto_manager.hpp"
#include "db_manager.hpp"
#include "password_helpers.hpp"
#include <cstring>
#include<SQLiteCpp/SQLiteCpp.h>
#include<SQLiteCpp/VariadicBind.h>

RegistrationWindow::RegistrationWindow()
    : m_showPassword(false), m_showConfirmPassword(false)
{
    memset(m_username, 0, sizeof(m_username));
    memset(m_email, 0, sizeof(m_email));
    memset(m_address, 0, sizeof(m_address));
    memset(m_password, 0, sizeof(m_password));
    memset(m_confirmPassword, 0, sizeof(m_confirmPassword));
}

void RegistrationWindow::Render(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(500, 450), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_FirstUseEver, ImVec2(0.5f, 0.5f));
    
    if (ImGui::Begin("User Registration", p_open, ImGuiWindowFlags_NoCollapse))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 8));
        
        ImGui::Text("Create Your Account");
        ImGui::Separator();
        ImGui::Spacing();
        
        ImGui::Text("Username:");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##username", m_username, IM_ARRAYSIZE(m_username));
        ImGui::Spacing();
        
        ImGui::Text("Email:");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputText("##email", m_email, IM_ARRAYSIZE(m_email));
        ImGui::Spacing();
        
        ImGui::Text("Address:");
        ImGui::SetNextItemWidth(-1);
        ImGui::InputTextMultiline("##address", m_address, IM_ARRAYSIZE(m_address), ImVec2(-1, 60));
        ImGui::Spacing();
        
        ImGui::Text("Password:");
        ImGui::SetNextItemWidth(-1);
        if (m_showPassword)
            ImGui::InputText("##password", m_password, IM_ARRAYSIZE(m_password));
        else
            ImGui::InputText("##password", m_password, IM_ARRAYSIZE(m_password), ImGuiInputTextFlags_Password);
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
        
        float buttonWidth = 150.0f;
        float spacing = 20.0f;
        float totalWidth = (buttonWidth * 2) + spacing;
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - totalWidth) * 0.5f);
        
        if (ImGui::Button("Register", ImVec2(buttonWidth, 40)))
        {
            // Validation
            if (strlen(m_username) == 0)
            {
                MessageBox::Show("Error", "Username cannot be empty", MessageBox::ERROR_MSG);
            }
            else if (strlen(m_email) == 0)
            {
                MessageBox::Show("Error", "Email cannot be empty", MessageBox::ERROR_MSG);
            }
            else if (strlen(m_password) == 0)
            {
                MessageBox::Show("Error", "Password cannot be empty", MessageBox::ERROR_MSG);
            }
            else if (!passes_password_test(m_password))
            {
               
                PasswordVerifier data = verify_password(m_password);
                string errMsgs;

                for (auto & msg : data.error_message)
                {
                    errMsgs += msg + "\n";
                }
                
                MessageBox::Show("Error", errMsgs, MessageBox::ERROR_MSG);
            }
            
            else if (strcmp(m_password, m_confirmPassword) != 0)
            {
                MessageBox::Show("Error", "Passwords do not match", MessageBox::ERROR_MSG);
            }
            else
            {
                
                User user;
                user.username = m_username;
                user.email = m_email;
                user.address = m_address;

                PasswordHandlers pwd;
                string password_hash = pwd.hash_password(m_password);
                user.password_hash = password_hash;

                DataEncryptionDecrytption KeyGen;
                KeyGen.store_key(KeyGen.generate_key());

                db_manager DBManager;
                SQLite::Database db(
                    "data/passwords.db",
                    SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
                );

                DBManager.register_user(user,db);
                
                MessageBox::Show("Success", "Registration successful! Please login.", MessageBox::SUCCESS);
                Reset();
                *p_open = false;
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

void RegistrationWindow::Reset()
{
    memset(m_username, 0, sizeof(m_username));
    memset(m_email, 0, sizeof(m_email));
    memset(m_address, 0, sizeof(m_address));
    memset(m_password, 0, sizeof(m_password));
    memset(m_confirmPassword, 0, sizeof(m_confirmPassword));
    m_showPassword = false;
    m_showConfirmPassword = false;
}