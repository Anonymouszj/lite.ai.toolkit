# 1. setup 3rd-party dependencies
message(STATUS "Setting up OpenCV libs for: ${CMAKE_CURRENT_SOURCE_DIR}")
set(OpenCV_LIBS
        opencv_core
        opencv_imgproc
        opencv_imgcodecs
        opencv_video
        opencv_videoio  # need ffmpeg for mp4 io
        ) # need only

message(STATUS "Setting up OpenCV libs done!")
