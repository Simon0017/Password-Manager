cmake_minimum_required(VERSION  3.21)

project(password_manager
        VERSION 0.0.0
        LANGUAGES CXX
)
add_executable(password_manager 
                src/main.cpp
                src/db_manager.cpp
                src/crypto_manager.cpp
                src/password_helpers.cpp
                src/imgui_impl_glfw.cpp
                src/imgui_impl_opengl3.cpp
                src/ui_manager.cpp
                src/registration_window.cpp
                src/login_window.cpp
                src/main_window.cpp
                src/save_password_window.cpp
                src/edit_password_window.cpp
                src/user_management_window.cpp
                src/message_box.cpp
)

# c++ starndard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# find dependencies
find_package(unofficial-sodium CONFIG REQUIRED)
find_package(SQLiteCpp CONFIG REQUIRED)
find_package(cryptopp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# link libraries
target_link_libraries(password_manager
    PRIVATE
        unofficial-sodium::sodium
        SQLiteCpp
        cryptopp::cryptopp
        imgui::imgui
        glfw
        OpenGL::GL
)


# include project header
target_include_directories(password_manager
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)