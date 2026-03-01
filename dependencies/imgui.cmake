include(FetchContent)

FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.5-docking
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(ImGui)

add_library(ImGui STATIC
    "${imgui_SOURCE_DIR}/imgui.cpp"
    "${imgui_SOURCE_DIR}/imgui_demo.cpp"
    "${imgui_SOURCE_DIR}/imgui_draw.cpp"
    "${imgui_SOURCE_DIR}/imgui_tables.cpp"
    "${imgui_SOURCE_DIR}/imgui_widgets.cpp"
)

target_include_directories(ImGui PUBLIC "${imgui_SOURCE_DIR}")
target_compile_definitions(ImGui PUBLIC IMGUI_DISABLE_DEFAULT_FONT)