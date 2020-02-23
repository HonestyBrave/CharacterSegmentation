file(GLOB_RECURSE Header "process/imgProcess.h")
file(GLOB_RECURSE Source "process/imgProcess.cpp")

set(OPEN_CV_INCLUDE "D:/install_soft/opencv-3.4.5-vc14_vc15/opencv/build/include")
set(OPEN0_CV_LIB_DIRECTOR "D:/install_soft/opencv-3.4.5-vc14_vc15/opencv/build/x64/vc15/lib")
set(LIB_NAME "opencv_world345")

include_directories(${OPEN_CV_INCLUDE})
link_directories(${OPEN0_CV_LIB_DIRECTOR})

set(CHARACTER characetersegmentaion)
add_library(${CHARACTER} SHARED ${Source} ${Header})

target_link_libraries(${CHARACTER} ${LIB_NAME})


