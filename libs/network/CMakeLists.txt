cmake_minimum_required(VERSION 3.15)

add_library(network INTERFACE)

target_include_directories(network INTERFACE 
  ${CMAKE_CURRENT_SOURCE_DIR}
)


target_link_libraries(network INTERFACE asio utils) 
