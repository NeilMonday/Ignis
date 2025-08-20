
cmake_minimum_required(VERSION 3.20)
project(Ignis LANGUAGES C CXX)

option(USE_VALIDATION "Enable Vulkan validation layers in debug builds" ON)
option(TRIPLE_BUFFERING "Prefer triple-buffered swapchain" OFF)
option(ENABLE_HDR "Try to use HDR colorspace when available" OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
  add_compile_options(/W4 /permissive- /Zc:preprocessor /EHsc)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Vulkan (use SDK on system)
find_package(Vulkan REQUIRED COMPONENTS glslc)

# ---- Submodules -----------------------------------------------------------
# GLFW
set(GLFW_DIR ${CMAKE_SOURCE_DIR}/external/glfw)
if(EXISTS "${GLFW_DIR}/CMakeLists.txt")
  set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
  set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
  add_subdirectory(${GLFW_DIR} EXCLUDE_FROM_ALL)
else()
  message(FATAL_ERROR "GLFW submodule missing. Run: git submodule update --init --recursive")
endif()

# GLM (header-only)
set(GLM_DIR ${CMAKE_SOURCE_DIR}/external/glm)
if(EXISTS "${GLM_DIR}/CMakeLists.txt")
  add_subdirectory(${GLM_DIR} EXCLUDE_FROM_ALL)
else()
  message(FATAL_ERROR "GLM submodule missing. Run: git submodule update --init --recursive")
endif()

# spdlog
set(SPDLOG_DIR ${CMAKE_SOURCE_DIR}/external/spdlog)
if(EXISTS "${SPDLOG_DIR}/CMakeLists.txt")
  add_subdirectory(${SPDLOG_DIR} EXCLUDE_FROM_ALL)
else()
  message(FATAL_ERROR "spdlog submodule missing. Run: git submodule update --init --recursive")
endif()

# VMA (headers only) -> interface target
set(VMA_DIR ${CMAKE_SOURCE_DIR}/external/VulkanMemoryAllocator)
if(EXISTS "${VMA_DIR}/include/vk_mem_alloc.h")
  add_library(VMA INTERFACE)
  target_include_directories(VMA INTERFACE ${VMA_DIR}/include)
else()
  message(FATAL_ERROR "VMA submodule missing. Run: git submodule update --init --recursive")
endif()

# ImGui
include(${CMAKE_SOURCE_DIR}/cmake/ImGui.cmake)

# ---- Engine ---------------------------------------------------------------

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/Window.cpp
  src/Renderer.cpp
  src/ImGuiLayer.cpp
  src/Scene.cpp
  src/GameObject.cpp
  src/Input.cpp
  src/Camera.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
  ${Vulkan_INCLUDE_DIRS}
  ${CMAKE_SOURCE_DIR}/external/imgui
  ${CMAKE_SOURCE_DIR}/external/imgui/backends
  ${CMAKE_SOURCE_DIR}/external/glm
  ${CMAKE_SOURCE_DIR}/external/VulkanMemoryAllocator/include
  ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
  Vulkan::Vulkan
  glfw
  glm::glm
  spdlog::spdlog
  imgui
  VMA
)

if(USE_VALIDATION)
  target_compile_definitions(${PROJECT_NAME} PRIVATE USE_VALIDATION=1)
endif()

if(TRIPLE_BUFFERING)
  target_compile_definitions(${PROJECT_NAME} PRIVATE TRIPLE_BUFFERING=1)
endif()

if(ENABLE_HDR)
  target_compile_definitions(${PROJECT_NAME} PRIVATE ENABLE_HDR=1)
endif()

# Shader stubs (optional build via glslc)
set(SHADER_SRC
  shaders/triangle.vert
  shaders/triangle.frag
)
foreach(SRC ${SHADER_SRC})
  configure_file(${SRC} ${SRC} COPYONLY)
endforeach()

if (TARGET glslc)
  foreach(SRC ${SHADER_SRC})
    get_filename_component(FILE_WE ${SRC} NAME_WE)
    set(SPV ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.spv)
    add_custom_command(
      OUTPUT ${SPV}
      COMMAND ${Vulkan_GLSLC_EXECUTABLE} -O ${CMAKE_CURRENT_SOURCE_DIR}/${SRC} -o ${SPV}
      DEPENDS ${SRC}
      COMMENT "Compiling ${SRC} -> ${SPV}"
      VERBATIM
    )
    list(APPEND SPV_BINARIES ${SPV})
  endforeach()
  add_custom_target(Shaders ALL DEPENDS ${SPV_BINARIES})
  add_dependencies(${PROJECT_NAME} Shaders)
endif()

if (WIN32)
  set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()
