cmake_minimum_required(VERSION 3.4.1)

# 定义ffmpeg总目录的变量
set(FFMPEG ${CMAKE_SOURCE_DIR}/ffmpeg)

# 定义 rtmp总目录的变量
set(RTMP ${CMAKE_SOURCE_DIR}/rtmp)

# 指定ffmpeg里面的头文件目录
include_directories(${FFMPEG}/include)

# 指定ffmpeg里面的库文件目录（静态库）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${FFMPEG}/libs/${CMAKE_ANDROID_ARCH_ABI}")

# 指定rtmp里面的库文件目录（静态库）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${RTMP}/libs/${CMAKE_ANDROID_ARCH_ABI}")


file(GLOB allCpp *.cpp)

add_library(
             myplayer
             SHARED
             ${allCpp} )

find_library(
              log-lib
              log )


target_link_libraries( # Specifies the target library.
        myplayer
        ${log-lib}
        # 此坑不想去踩了，直接可以忽略顺序，为所欲为的填写即可
        -Wl,--start-group
        avcodec avfilter avformat avutil swresample swscale
        -Wl,--end-group

        z
        rtmp)