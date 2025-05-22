cmake_minimum_required(VERSION 3.10)

project(RiscV_Siumlator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(SRC_DIR "src")
set(INCLUDE_DIR "src/include")
set(TEST_DIR "test")

file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE TEST_FILES "${TEST_DIR}/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES})

include_directories(${INCLUDE_DIR})

add_compile_options(-Wall -Wextra -pedantic -frounding-math -ffloat-store -g -O3)

target_link_libraries(${PROJECT_NAME} m)

option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
if(ENABLE_ASAN)
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
    target_link_libraries(${PROJECT_NAME} PRIVATE -fsanitize=address)
endif()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
list(REMOVE_ITEM SRC_FILES "${CMAKE_SOURCE_DIR}/src/main.cpp")
add_executable(tests ${SRC_FILES} ${TEST_FILES})
target_link_libraries(tests GTest::GTest GTest::Main pthread)

add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(debug
    COMMAND gdb ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(valgrind
    COMMAND valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

add_custom_target(test_run
    COMMAND ./tests
    DEPENDS tests
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} Doxyfile
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating documentation with Doxygen"
    )
endif()

message(STATUS "Project configured successfully! Run 'make' to build.")
