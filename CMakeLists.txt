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
  src/view/MatViewShader.cpp
  src/view/MatView.cpp
  src/capture/AbstractCapture.cpp
  src/capture/SimulationCapture.cpp
  src/capture/FlyCaptureCamera.cpp
  src/tracking/BallDetector.cpp
  src/tracking/BallFollower.cpp
  src/tracking/BallTracker.cpp
  src/view/Label.cpp
  src/view/BallMenController.cpp
  src/view/ballman/BallMan.cpp
  src/view/ballman/Leg.cpp
  src/view/ballman/Face.cpp
  src/view/primitives/Line.cpp
  ${MatView_RESOURCES})

target_include_directories(EO PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_include_directories(EO PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(EO
  ${FlyCapture2_LIBRARIES}
  opencv_core
  opencv_imgproc
  opencv_highgui
  opencv_imgcodecs
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
