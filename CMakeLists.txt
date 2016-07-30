cmake_minimum_required(VERSION 2.8.12)
project(HelloMagnum)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

add_subdirectory(libs/corrade)
add_subdirectory(libs/magnum)

find_package(FlyCapture2 REQUIRED)
find_package(OpenCV REQUIRED )
find_package(Magnum REQUIRED Sdl2Application)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

#set(MAGNUM_PLUGINS_IMPORTER_DEBUG_DIR, ../libs/magnum/src/MagnumPlugins)
#set(MAGNUM_PLUGINS_IMPORTER_DIR, ../libs/magnum/src/MagnumPlugins)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/configure.h.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/configure.h)

corrade_add_resource(CameraView_RESOURCES res/resources.conf)

add_executable(HelloMagnum
  src/App.cpp
  src/CameraViewShader.cpp
  src/CameraView.cpp
  src/FlyCaptureCamera.cpp
  ${CameraView_RESOURCES})

target_include_directories(HelloMagnum PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(HelloMagnum
  ${FlyCapture2_LIBRARIES}
  ${OpenCV_LIBS}
  Magnum::Application
  Magnum::Magnum
  Magnum::Shaders
  Magnum::Primitives
  Magnum::MeshTools)

add_custom_target(Run COMMAND
  HelloMagnum DEPENDS
  HelloMagnum WORKING_DIRECTORY
  ${CMAKE_PROJECT_DIR} )
