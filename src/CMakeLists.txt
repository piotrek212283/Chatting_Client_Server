cmake_minimum_required(VERSION 3.5.0)

add_library(sources STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/test_client.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/test_server.cpp

)

target_include_directories(sources PUBLIC 
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(sources PUBLIC
  ${COMPILE_OPTIONS}
)

target_link_libraries(sources PRIVATE 
  asio
  utils
)

