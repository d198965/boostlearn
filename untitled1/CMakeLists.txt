cmake_minimum_required(VERSION 3.7)
project(untitled1)

set(CMAKE_CXX_STANDARD 11)

set(FCGI "/usr/local/include/")
set(BOOST_ROOT "/usr/local/include/boost")
set(Boost_LIBRARIES "/usr/local/lib")

set(SOURCE_FILES main.cpp asio_server.cpp session.cpp protlcal.h)

#添加头文件搜索路径
include_directories(/usr/local/include)

#添加库文件搜索路径
link_directories(/usr/local/lib)

add_executable(untitled1 ${SOURCE_FILES})

target_link_libraries(untitled1 FCGI)

target_link_libraries(untitled1 boost_system)

target_link_libraries(untitled1 boost_serialization)

#find_library(PATH_TO_BoostSerialization boost_serialization /usr/local)
#target_link_libraries(untitled1 ${PATH_TO_BoostSerialization})
