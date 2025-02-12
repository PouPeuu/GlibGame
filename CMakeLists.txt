cmake_minimum_required(VERSION 3.20)
project(GlibGame CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR src)
set(HEADER_DIR headers)
set(RES_DIR res)

file(GLOB SOURCES "${SRC_DIR}/*.cpp")

find_package(DissensionFramework REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE DissensionFramework)

include_directories(${HEADER_DIR})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/res
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)