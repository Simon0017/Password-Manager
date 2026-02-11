#include<iostream>
#include<SQLiteCpp/SQLiteCpp.h>
#include<sodium.h>

#include<imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include"models.hpp"
#include"ui_manager.h"

using namespace std;

int main(){

    if (sodium_init() < 0)
    {
        cerr << "Libsodium init failed";
        return 1;
    }
    

    SQLite::Database db(
        "data/passwords.db",
        SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE
    );

    // Initialize GLFW
    if (!glfwInit())
        return -1;

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Create window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Password Manager", nullptr, nullptr);
    if (window == nullptr)
        return -1;
    
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Setup style
    ImGui::StyleColorsDark();
    
    // Customize colors
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.12f, 1.0f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.12f, 0.14f, 1.0f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.2f, 0.22f, 1.0f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.3f, 0.32f, 1.0f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.25f, 0.25f, 0.27f, 1.0f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.17f, 1.0f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.2f, 0.2f, 0.22f, 1.0f);
    colors[ImGuiCol_Button] = ImVec4(0.25f, 0.5f, 0.75f, 1.0f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.3f, 0.6f, 0.85f, 1.0f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.2f, 0.4f, 0.65f, 1.0f);
    colors[ImGuiCol_Header] = ImVec4(0.25f, 0.5f, 0.75f, 0.8f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.6f, 0.85f, 0.8f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.2f, 0.4f, 0.65f, 0.8f);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Create UI Manager
    UIManager uiManager;
    
    // TODO: Check if user is already logged in (session management)
    // If logged in, set state to MAIN_DASHBOARD
    // Otherwise start with LOGIN screen
    uiManager.SetAppState(UIManager::AppState::LOGIN);

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Render UI
        uiManager.Render();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
