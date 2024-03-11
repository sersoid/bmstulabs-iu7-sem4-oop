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

qt_add_executable(${ProjectId}
        ${SRC_FILES}
        ${UI_FILES}
)

target_include_directories(${ProjectId} PRIVATE
        ${INC_DIR}
)

target_link_libraries(${ProjectId} PUBLIC
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

set_target_properties(${ProjectId} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${OUT_DIR}
        WIN32_EXECUTABLE TRUE
)
