#pragma once
#include <string>

class MessageBox
{
public:
    enum Type
    {
        INFO,
        WARNING,
        ERROR_MSG,
        SUCCESS
    };

    static void Show(const std::string& title, const std::string& message, Type type = INFO);
    static void Render();
    static bool IsOpen();

private:
    static bool s_isOpen;
    static std::string s_title;
    static std::string s_message;
    static Type s_type;
};