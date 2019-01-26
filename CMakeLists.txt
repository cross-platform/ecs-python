cmake_minimum_required(VERSION 3.1)

project(EcsPython)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -W4")
elseif(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -pedantic -Wall -Wextra -Wnon-virtual-dtor -Wno-unknown-pragmas")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -pthread -pedantic -Wall -Wextra -Wnon-virtual-dtor")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-return-type-c-linkage -Wno-gnu-zero-variadic-macro-arguments -Wno-vla-extension")
endif()

enable_testing()

find_package(PythonLibs 2.7 EXACT)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${PYTHON_INCLUDE_DIRS}
)

file(GLOB srcs src/*.cpp)
file(GLOB hdrs include/*.h)

add_library(
    ${PROJECT_NAME}
    ${srcs}
    ${hdrs}
)

target_link_libraries(
    ${PROJECT_NAME}
    ${PYTHON_LIBRARIES}
)

add_subdirectory(example)
add_subdirectory(tests)

install(
    TARGETS ${PROJECT_NAME}
    DESTINATION lib
)

install(
    FILES ${hdrs}
    DESTINATION include
)
