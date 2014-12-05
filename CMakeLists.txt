cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread -Wall -Wextra")

project(EcsPython)

add_subdirectory(example)

if(UNIX)
    find_package(PythonLibs REQUIRED)
endif(UNIX)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")    
    set(PYTHON_LIBRARIES "C:/Python27/libs/python27.lib")
    set(PYTHON_INCLUDE_DIRS "C:/Python27/include")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

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
