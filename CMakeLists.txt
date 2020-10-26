cmake_minimum_required(VERSION 3.1)
project(tp_c_cpp_iz2)

set(CMAKE_CXX_STANDARD 14)

#static library
add_library(${PROJECT_NAME}_lib STATIC lib/consistent/consistent.c)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${PROJECT_SOURCE_DIR}/lib/consistent)

#dynamic library
add_library(${PROJECT_NAME}_dlib SHARED lib/parallel/parallel.c lib/consistent/consistent.c)
target_include_directories(${PROJECT_NAME}_dlib PUBLIC ${PROJECT_SOURCE_DIR}/lib/parallel ${PROJECT_SOURCE_DIR}/lib/consistent)
target_link_libraries(${PROJECT_NAME}_dlib Threads::Threads )

file(GLOB sources "${PROJECT_SOURCE_DIR}/src/*.c")

add_executable(${PROJECT_NAME} ${sources})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_lib
        ${PROJECT_NAME}_dlib ${CMAKE_DL_LIBS})