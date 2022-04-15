#########################################################################

# CMAKE MINIMAL VERSION
cmake_minimum_required (VERSION 3.8)

# PROJECT PROPERTIES
project("CMM compiler" LANGUAGES CXX)
set(CMAKE_CXX_FLAGS "-m32")

#########################################################################

# ASSEMBLY OPTIONS

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

add_compile_options(
    -Wall
    -Wpedantic

    -Wno-multichar
    -Wno-format-contains-nul
    -Wno-sign-compare
)

#########################################################################

set(COMPILER_LIB_SRC "compiler_lib_src")

# Simple and fast assembler library (sfasmlib)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/sfasmlib/sfasmlib.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# TEXT LIBRARY

set(TEXT_LIB_PATH)
set(TEXT_LIB_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${COMPILER_LIB_SRC}/libtext)

find_library(TEXT_LIB_PATH NAMES text PATHS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/${NCPU_DIR}/libtext)

if (NOT TEXT_LIB_PATH)
    set(TEXT_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/libtext)
    add_subdirectory(${TEXT_LIB_SRC_DIR} ${TEXT_LIB_DIR} ${PARENT_SCOPE})
    set(TEXT_LIB_PATH text)
endif()

# TREE

set(TREE_LIB_NAME "tree")
set(TREE_LIB_PATH ${TREE_LIB_NAME})
set(TREE_LIB_SRC_DIR ${TREE_LIB_NAME})

set(TREE_LIB_SRC
    tree/tree.cpp
)

add_library(${TREE_LIB_NAME} STATIC)
target_include_directories(${TREE_LIB_NAME} PRIVATE ${TREE_LIB_NAME})
target_sources(${TREE_LIB_NAME} PRIVATE ${TREE_LIB_SRC})

# CMM FRONTEND

set(LANG_FRONTEND_NAME "cmm_frontend")

set(LANG_FRONTEND_SRC
    frontend/main.cpp
    frontend/parsing.cpp
    frontend/tree_recording.cpp
)

add_executable(${LANG_FRONTEND_NAME} ${LANG_FRONTEND_SRC})

target_include_directories(${LANG_FRONTEND_NAME} PRIVATE ${TEXT_LIB_SRC_DIR} PRIVATE ${TREE_LIB_SRC_DIR})
target_link_libraries(${LANG_FRONTEND_NAME} ${TEXT_LIB_PATH} ${TREE_LIB_PATH})

# LANG OPTIMIZER

set(LANG_OPTIMIZER_NAME "lang_optimizer")

set(LANG_OPTIMIZER_SRC
    optimizer/main.cpp
    optimizer/derivative.cpp
    optimizer/optimizer.cpp
    optimizer/tree_reading.cpp
    optimizer/tree_recording.cpp
)

add_executable(${LANG_OPTIMIZER_NAME} ${LANG_OPTIMIZER_SRC})

target_include_directories(${LANG_OPTIMIZER_NAME} PRIVATE ${TEXT_LIB_SRC_DIR}  PRIVATE ${TREE_LIB_SRC_DIR})
target_link_libraries(${LANG_OPTIMIZER_NAME} ${TEXT_LIB_PATH} ${TREE_LIB_PATH})

# LANG REVERSED FRONTEND

set(CMM_REV_FRONTEND_NAME "cmm_frontend_rev")

set(CMM_REV_FRONTEND_SRC
    frontend_reversed/main.cpp
    frontend_reversed/frontend_rev.cpp
    frontend_reversed/tree_reading.cpp
)

add_executable(${CMM_REV_FRONTEND_NAME} ${CMM_REV_FRONTEND_SRC})

target_include_directories(${CMM_REV_FRONTEND_NAME} PRIVATE ${TEXT_LIB_SRC_DIR} PRIVATE ${TREE_LIB_SRC_DIR})
target_link_libraries(${CMM_REV_FRONTEND_NAME} ${TEXT_LIB_PATH} ${TREE_LIB_PATH})

# LANG_COMPILE

set(LANG_COMPILE_NAME "lang_compile")

set(LANG_COMPILE_SRC
    backend/main.cpp
    backend/backend.cpp
    backend/exe_creator.cpp
    backend/tree_reading.cpp
)

add_executable(${LANG_COMPILE_NAME} ${LANG_COMPILE_SRC})

target_include_directories(${LANG_COMPILE_NAME} PRIVATE ${TEXT_LIB_SRC_DIR} PRIVATE ${TREE_LIB_SRC_DIR})
target_link_libraries(${LANG_COMPILE_NAME} ${TEXT_LIB_PATH} ${TREE_LIB_PATH})

##########################################################################
