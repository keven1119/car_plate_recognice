# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# 创建一个变量 source_file 它的值就是src/main/cpp/ 所有的.cpp文件
file(GLOB source_file src/main/cpp/*.cpp)
file(GLOB recgnize_source_file src/main/cpp/recgnize/*.cpp)

add_library( # Sets the name of the library.
             car-plate-recognice

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
        ${source_file}
        ${recgnize_source_file}
        )

include_directories(../carplate/src/main/cpp/include)
include_directories(../carplate/src/main/cpp/recgnize)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -L${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}" )


target_link_libraries( # Specifies the target library.
        car-plate-recognice
        opencv_java3

        # Links the target library to the log library
                       # included in the NDK.
                       )