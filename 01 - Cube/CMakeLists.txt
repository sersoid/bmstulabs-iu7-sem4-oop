cmake_minimum_required(VERSION 3.5)

get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " - " "_" ProjectId ${ProjectId})
string(REPLACE " " "_" ProjectId ${ProjectId})

set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/inc)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(UI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ui)
set(OUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/out)

file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)
file(GLOB UI_FILES ${UI_DIR}/*.ui)

#DEBUG

qt_add_executable(${ProjectId}_Debug
        ${SRC_FILES}
        ${UI_FILES}
)

target_include_directories(${ProjectId}_Debug PRIVATE
        ${INC_DIR}
)

target_link_libraries(${ProjectId}_Debug PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

set_target_properties(${ProjectId}_Debug PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR}
        WIN32_EXECUTABLE TRUE
)

target_compile_options(${ProjectId}_Debug PUBLIC -g -DDEBUG)

#RELEASE

qt_add_executable(${ProjectId}_Release
        ${SRC_FILES}
        ${UI_FILES}
)

target_include_directories(${ProjectId}_Release PRIVATE
        ${INC_DIR}
)

target_link_libraries(${ProjectId}_Release PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

set_target_properties(${ProjectId}_Release PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR}
        WIN32_EXECUTABLE TRUE
)

target_compile_options(${ProjectId}_Release PUBLIC -DNDEBUG)
