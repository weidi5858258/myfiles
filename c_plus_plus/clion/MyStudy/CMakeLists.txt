cmake_minimum_required(VERSION 3.9)
project(MyStudy)

set(CMAKE_CXX_STANDARD 11)

add_executable(MyStudy
        main.cpp
        src/Test.cpp
        include/Test.h
        include/MyHead.h
        src/B0.cpp
        src/Pointer.cpp
        src/TestLibJpegTurbo.cpp
        )

################################################################

#add_library(libjpeg
#        libturbojpeg
#        STATIC
#        IMPORTED
#        )
#set_target_properties(libjpeg
#        libturbojpeg
#        PROPERTIES IMPORTED_LOCATION
#        /opt/libjpeg-turbo/lib32/libjpeg.a
#        /opt/libjpeg-turbo/lib32/libturbojpeg.a
#        )

link_libraries(
        /opt/libjpeg-turbo/lib32/libjpeg.a
        /opt/libjpeg-turbo/lib32/libturbojpeg.a
)

#link_directories(
#        /opt/libjpeg-turbo/lib32/libjpeg.a
#        /opt/libjpeg-turbo/lib32/libturbojpeg.a
#)

# 设置c++编译器
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#set(OpenCV_DIR /usr/local/share/OpenCV)

# 将所有的源文件列为一个集合，集合名字叫做SRC_LISTS
#set(SRC_LISTS
#        ./src/Test.cpp
#        )

# 将集合里的所有的源文件生成一个静态库，该静态库的名字libsugan，
# 注意，在整个CmakeLists里都要用libsugan这个名字来代替之前那个集合生成的库。
#add_library(lib_test
#        ${SRC_LISTS}
#        )

# Find OpenCV, you may need to set OpenCV_DIR variable
# to the absolute path to the directory containing
# OpenCVConfig.cmake file via the command line or GUI
# 在整个电脑上找opencv包
#find_package(OpenCV REQUIRED)

# If the package has been found, several variables will
# be set, you can find the full list with descriptions
# in the OpenCVConfig.cmake file.
# Print some message showing some of them
#message(STATUS "OpenCV library status:")
# version: 3.3.0
#message(STATUS "    version: ${OpenCV_VERSION}")

# libraries:
# opencv_calib3d;opencv_core;opencv_dnn;
# opencv_features2d;opencv_flann;opencv_highgui;
# opencv_imgcodecs;opencv_imgproc;opencv_ml;
# opencv_objdetect;opencv_photo;opencv_shape;
# opencv_stitching;opencv_superres;opencv_video;
# opencv_videoio;opencv_videostab
#message(STATUS "    libraries: ${OpenCV_LIBS}")

# include path: /usr/local/include;/usr/local/include/opencv
#message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

# Add OpenCV headers location to your include paths
# 包含头文件路径
#include_directories(${OpenCV_INCLUDE_DIRS}
#        ./include/
#        )

# Link your application with OpenCV libraries
# 链接静态库需要的依赖库
#target_link_libraries(lib_test
#        ${OpenCV_LIBS}
#        )

