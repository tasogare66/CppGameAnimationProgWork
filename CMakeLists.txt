# CMakeList.txt : CppGameAnimationProgWork の CMake プロジェクト。ソースを含めて、次を定義します:
# プロジェクト専用ロジックはこちらです。
#
cmake_minimum_required (VERSION 3.8)

# サポートされている場合は、MSVC コンパイラのホット リロードを有効にします。
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# use custom file to find libraries
if(WIN32)
  list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
endif()

project ("CppGameAnimationProgWork")

# ソースをこのプロジェクトの実行可能ファイルに追加します。
add_executable (CppGameAnimationProgWork "Main.cpp" "Main.h" "tools/Logger.cpp" "tools/Logger.h" "window/Window.cpp" "window/Window.h")

find_package(glfw3 3.3 REQUIRED)

# variable is set by FindGLFW3.cmake, reuse for Linux
if(UNIX)
  set(GLFW3_LIBRARY glfw)
endif()

include_directories(${GLFW3_INCLUDE_DIR} tools window)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET CppGameAnimationProgWork PROPERTY CXX_STANDARD 20)
endif()

if(MSVC)
  target_link_libraries(CppGameAnimationProgWork ${GLFW3_LIBRARY})
else()
  # Clang and GCC may need libstd++
  target_link_libraries(CppGameAnimationProgWork ${GLFW3_LIBRARY} stdc++)
endif()

# TODO: テストを追加し、必要な場合は、ターゲットをインストールします。
