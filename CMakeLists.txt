cmake_minimum_required(VERSION 3.0)

project(SocketCAN-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)  # enable C++11 standard

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/include/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        )
       
#add_executable defines our binary with all linked source files.
add_executable(mainWindow MainWindow.cpp)
add_library(CanWrapper OBJECT include/CanWrapper.h)

#install tells cmake to install our binary into the bin directory of the install
install(TARGETS mainWindow DESTINATION bin)
install(FILES include/CanWrapper.h DESTINATION include)
install(FILES src/CanWrapper.cpp DESTINATION src)

