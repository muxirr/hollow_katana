cmake_minimum_required(VERSION 3.20)

project(hollow_katana)

set(CMAKE_CXX_STANDARD 20)
set(VS_DIR "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Auxiliary/VS")

add_definitions(-D_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING)

#aux_source_directory(./source SRC)
file(GLOB SRC "./source/*.cpp" "./source/*.c")

include_directories(include ${VS_DIR}/include)
link_directories(${VS_DIR}/lib/x64)

add_executable(${PROJECT_NAME} WIN32 ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC EasyXa)