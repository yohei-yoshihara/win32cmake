cmake_minimum_required(VERSION 3.28 FATAL_ERROR)
project(win32cmake LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
add_compile_options(/w44996)

add_subdirectory(spdlog-1.13.0)

include(sourcelist.cmake)
add_executable(win32cmake WIN32
  src/Main.hpp
  src/Main.cpp
  src/App.hpp
  src/App.cpp
  src/CommandHandler.hpp
  src/CommandHandler.cpp
  src/App.rc
  src/Resource.h

  src/ribbonres.h
  src/ribbonres.rc
  src/ribbonmarkup.bml

  src/tinyxml2.cpp
  src/tinyxml2.h
  )
source_group("Resource Files" FILES src/App.rc src/Resource.h)
source_group("TinyXML2 Files" FILES src/tinyxml2.cpp src/tinyxml2.h)
source_group("Ribbon Resource Files" FILES src/ribbonres.h src/ribbonres.rc src/ribbonmarkup.bml)
target_compile_options(win32cmake PRIVATE /W4 /WX)
target_compile_definitions(win32cmake PUBLIC UNICODE _UNICODE)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT win32cmake)
target_link_libraries(win32cmake spdlog)
target_include_directories(win32cmake SYSTEM PRIVATE spdlog-1.13.0)

target_link_options(win32cmake PRIVATE "/DYNAMICBASE:NO")
target_link_options(win32cmake PRIVATE "/DELAYLOAD:user32.dll")
target_link_options(win32cmake PRIVATE "/DELAYLOAD:shell32.dll")
target_link_options(win32cmake PRIVATE "/DELAYLOAD:uxtheme.dll")
target_link_options(win32cmake PRIVATE 
  "uuid.lib" "propsys.lib" "kernel32.lib" "user32.lib" "gdi32.lib" 
  "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" 
  "oleaut32.lib" "odbc32.lib" "odbccp32.lib" "uxtheme.lib" "delayimp.lib")

add_custom_command(
  PRE_BUILD
  OUTPUT  ${CMAKE_CURRENT_SOURCE_DIR}/src/ribbonmarkup.bml 
          ${CMAKE_CURRENT_SOURCE_DIR}/src/ribbonres.rc 
          ${CMAKE_CURRENT_SOURCE_DIR}/src/ribbonres.h
  COMMAND "$ENV{WindowsSdkVerBinPath}/x86/UICC" ribbonmarkup.xml 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/ribbonmarkup.bml 
            /header:${CMAKE_CURRENT_SOURCE_DIR}/src/ribbonres.h 
            /res:${CMAKE_CURRENT_SOURCE_DIR}/src/ribbonres.rc
  DEPENDS src/ribbonmarkup.xml
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src
)
