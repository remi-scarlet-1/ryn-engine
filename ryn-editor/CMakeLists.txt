project(RynEditor)

add_executable(RynEditor)
ryn_compile_options(RynEditor)

target_compile_definitions(RynEditor
    PRIVATE
    RYN_EDITOR
)

target_include_directories(RynEditor
    PRIVATE source
)

target_link_libraries(RynEditor
    PRIVATE
    RynRuntime
    ImGui
)

target_sources(RynEditor
    PRIVATE FILE_SET CXX_MODULES BASE_DIRS source FILES
    source/core/__BUNDLE__.ixx
    source/core/EditorPanel.ixx
    source/core/TypeCategory.ixx

    source/detail/__BUNDLE__.ixx
    source/detail/ImGuiBackend.ixx

    source/panel/__BUNDLE__.ixx
    source/panel/Hierarchy.ixx
    source/panel/Registry.ixx

    source/service/__BUNDLE__.ixx
    source/service/Editor.ixx

    PRIVATE
    source/detail/FontEmbeddings.inl
    source/detail/ImGuiBackend.cpp

    source/service/Editor.cpp

    source/Main.cpp
)