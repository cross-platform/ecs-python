project(EcsPythonTests)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB srcs *.cpp)
file(GLOB hdrs *.hpp)

add_executable(
    ${PROJECT_NAME}
    ${srcs}
    ${hdrs}
)

target_link_libraries(
    ${PROJECT_NAME}
    EcsPython
)

add_test(
    ${PROJECT_NAME}
    ${PROJECT_NAME}
)
