cmake_minimum_required(VERSION 3.15)

add_library(utils INTERFACE)

target_include_directories(utils INTERFACE 
  ${CMAKE_CURRENT_SOURCE_DIR}
)


target_link_libraries(utils INTERFACE asio) 
