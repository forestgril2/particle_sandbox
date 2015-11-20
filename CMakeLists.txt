cmake_minimum_required(VERSION 2.6)
project(lennardnet)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(LennardNet_SRCS LennardNet.cpp main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

qt4_automoc(${LennardNet_SRCS})
add_executable(lennardnet ${LennardNet_SRCS})
target_link_libraries(lennardnet ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

install(TARGETS lennardnet RUNTIME DESTINATION bin)
