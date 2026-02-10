#include "message_box.h"
#include "imgui.h"

bool MessageBox::s_isOpen = false;
std::string MessageBox::s_title = "";
std::string MessageBox::s_message = "";
MessageBox::Type MessageBox::s_type = MessageBox::INFO;

void MessageBox::Show(const std::string& title, const std::string& message, Type type)
{
    s_title = title;
    s_message = message;
    s_type = type;
    s_isOpen = true;
}

void MessageBox::Render()
{
    if (!s_isOpen)
        return;

    ImGui::SetNextWindowSize(ImVec2(400, 150), ImGuiCond_FirstUseEver);
    if (ImGui::Begin(s_title.c_str(), &s_isOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImVec4 color;
        const char* icon;
        
        switch (s_type)
        {
            case INFO:
                color = ImVec4(0.3f, 0.6f, 1.0f, 1.0f);
                icon = "[INFO]";
                break;
            case WARNING:
                color = ImVec4(1.0f, 0.8f, 0.0f, 1.0f);
                icon = "[WARNING]";
                break;
            case ERROR_MSG:
                color = ImVec4(1.0f, 0.2f, 0.2f, 1.0f);
                icon = "[ERROR]";
                break;
            case SUCCESS:
                color = ImVec4(0.2f, 1.0f, 0.2f, 1.0f);
                icon = "[SUCCESS]";
                break;
        }
        
        ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextWrapped("%s", icon);
        ImGui::PopStyleColor();
        ImGui::SameLine();
        ImGui::TextWrapped("%s", s_message.c_str());
        
        ImGui::Separator();
        
        float buttonWidth = 100.0f;
        ImGui::SetCursorPosX((ImGui::GetWindowWidth() - buttonWidth) * 0.5f);
        if (ImGui::Button("OK", ImVec2(buttonWidth, 0)))
        {
            s_isOpen = false;
        }
    }
    ImGui::End();
}

bool MessageBox::IsOpen()
{
    return s_isOpen;
}