cmake_minimum_required(VERSION 3.16.0)
project(Chatting_Client_Server VERSION 1.0.0)

enable_language(C)
enable_language(CXX)

set(COMPILE_OPTIONS
  $<$<COMPILE_LANGUAGE:CXX>:
    -std=c++17
  >
  $<$<CONFIG:Release>:-O3>
  $<$<CONFIG:Debug>:-Og>
  -fdiagnostics-color=always
)

add_subdirectory(libs)
add_subdirectory(src)

add_executable(Chatting_Client_Server main.cpp)


target_link_libraries(${PROJECT_NAME} PUBLIC 
  asio
  sources
)



