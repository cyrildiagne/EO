cmake_minimum_required(VERSION 2.8.12)
project(EO)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/")

add_subdirectory(libs/corrade)
add_subdirectory(libs/magnum)
add_subdirectory(libs/magnum-plugins)

find_package(FlyCapture2 REQUIRED)
find_package(OpenCV REQUIRED )
find_package(Magnum REQUIRED Sdl2Application)
find_package(MagnumPlugins REQUIRED FreeTypeFont)

set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/configure.h.cmake
               ${CMAKE_CURRENT_BINARY_DIR}/configure.h)

corrade_add_resource(MatView_RESOURCES res/resources.conf)

add_executable(EO
  src/App.cpp
  src/MatViewShader.cpp
  src/MatView.cpp
  src/FlyCaptureCamera.cpp
  src/BallTracker.cpp
  src/Label.cpp
  src/BallMan.cpp
  src/Leg.cpp
  src/Line.cpp
  ${MatView_RESOURCES})

target_include_directories(EO PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(EO
  ${FlyCapture2_LIBRARIES}
  ${OpenCV_LIBS}
  Magnum::Application
  Magnum::Magnum
  Magnum::Shaders
  Magnum::Primitives
  Magnum::MeshTools
  Magnum::Text)

add_custom_target(Run
  COMMAND EO
  DEPENDS EO
  DEPENDS FreeTypeFont
  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR} )
