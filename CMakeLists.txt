cmake_minimum_required(VERSION 2.8)

project(EcsPython)

add_definitions(
    -fpermissive
)

add_subdirectory(example)

find_package(PythonLibs REQUIRED)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${PYTHON_INCLUDE_DIRS}
)

file(GLOB srcs src/*.cpp)
file(GLOB hdrs include/*.h)
file(GLOB in_hdrs include/ecspython/*.h)
file(GLOB dsp_hdrs include/dspatch/*.h)

add_library(
    ${PROJECT_NAME}
    ${srcs}
    ${hdrs}
    ${in_hdrs}
    ${dsp_hdrs}
)

target_link_libraries(
    ${PROJECT_NAME}
    ${PYTHON_LIBRARIES}
)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION lib
)

install(
    FILES ${hdrs}
    DESTINATION include
)

install(
    FILES ${in_hdrs}
    DESTINATION include/ecspython
)

install(
    FILES ${dsp_hdrs}
    DESTINATION include/dspatch
)
