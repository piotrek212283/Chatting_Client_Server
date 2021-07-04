cmake_minimum_required(VERSION 3.15)

add_library(asio INTERFACE)

target_include_directories(asio INTERFACE 
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)


target_link_libraries(asio INTERFACE pthread) 
