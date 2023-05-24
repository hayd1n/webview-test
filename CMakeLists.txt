cmake_minimum_required(VERSION 3.18)

project(WebviewTest C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(APPLE)
  add_link_options(-framework WebKit)
endif()

set(WEBVIEW_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/libs/webview)

set(MAIN_SOURCES
  ${PROJECT_SOURCE_DIR}/basic.cc)

add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE ${MAIN_SOURCES})

# add_executable(${CMAKE_PROJECT_NAME} ${MAIN_SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${WEBVIEW_INCLUDE_DIR})

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
  BUNDLE True
  MACOSX_BUNDLE_GUI_IDENTIFIER edu.ntust.${CMAKE_PROJECT_NAME}
  MACOSX_BUNDLE_BUNDLE_NAME ${CMAKE_PROJECT_NAME}
  MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
  MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
  MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/Info.plist.in
)

if(APPLE)
  # Enable High-DPI on macOS through our custom Info.plist template
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Info.plist.in)
endif()

if(MSVC)
  # Tell MSVC to use main instead of WinMain for Windows subsystem executables
  set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES LINK_FLAGS "/ENTRY:mainCRTStartup")
endif()

# Set the assets path to "/assets" or "/../Resources/assets" on macOS
if(APPLE)
  set(ASSETS_PATH "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/../Resources/assets")
else()
  set(ASSETS_PATH "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/assets")
endif()

# Copy assets to assets path
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets/" "${ASSETS_PATH}")