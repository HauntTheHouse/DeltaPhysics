cmake_minimum_required(VERSION 3.20)
project(DeltaGraphics)

file(GLOB_RECURSE INCLUDE_LIST CONFIGURE_DEPENDS
    "include/*.hpp"
)

file(GLOB SRC_LIST CONFIGURE_DEPENDS
    "src/EngineCore/*.cpp"
    "src/EngineCore/Modules/*.cpp"
)

option(USE_OPENGL "Using OpenGL as rendering system" ON)
option(USE_VULKAN "Using Vulkan as rendering system" OFF)

if(USE_OPENGL AND USE_VULKAN)
    message("Using only one rendering system is allowed. Using OpenGL as rendering system")
    set(USE_OPENGL ON)
    set(USE_VULKAN OFF)
endif()

if(NOT USE_OPENGL AND NOT USE_VULKAN)
    message("Using at least one rendering system is required. Using OpenGL as rendering system")
    set(USE_OPENGL ON)
    set(USE_VULKAN OFF)
endif()

if(USE_VULKAN)
    message("Only one rendering system is currently allowed. Using OpenGL as rendering system")
    set(USE_OPENGL ON)
    set(USE_VULKAN OFF)
endif()

if(USE_OPENGL)
    message("Using OpenGL as rendering system")
    file(GLOB OPENGL_LIST CONFIGURE_DEPENDS
        "src/EngineCore/Rendering/OpenGL/*.cpp"
        "shaders/*.vert"
        "shaders/*.frag"
    )
    list(APPEND SRC_LIST ${OPENGL_LIST})
endif()

add_library(${PROJECT_NAME} STATIC ${INCLUDE_LIST} ${SRC_LIST})

cmrc_add_resource_library(ShaderResources
        NAMESPACE shaders
        shaders/object.vert
        shaders/object.frag)

target_link_libraries(${PROJECT_NAME} glfw glad spdlog imgui DeltaMath ShaderResources)
target_include_directories(${PROJECT_NAME} PUBLIC "include")

target_precompile_headers(${PROJECT_NAME} PUBLIC "include/EngineCore/PCH.hpp")

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INCLUDE_LIST} ${SRC_LIST})
