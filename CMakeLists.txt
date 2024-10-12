cmake_minimum_required(VERSION 3.29)


project(TerraTE VERSION 1.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

add_compile_options(-Wall -Wextra -g)

include_directories(src/headers)

add_executable(TerraTE
    src/headers/utilities.c
    src/main.c
)

# target_link_libraries(TerraTE PRIVATE ${PROJECT_SOURCE_DIR}/src/headers/pdcurses.a)
target_link_libraries(TerraTE PRIVATE -lwinmm -lgdi32)

target_include_directories(TerraTE PUBLIC
        ${PROJECT_SOURCE_DIR}/src/headers
)

set_target_properties(TerraTE PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/dist
)