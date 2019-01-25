project(EcsPythonExample)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    EcsPython
)
