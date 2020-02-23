set(TEST_CHARACTER test_character)
add_executable(${TEST_CHARACTER} test_character.cpp)

set(OpenCV_LIBS "D:/install_soft/opencv-3.4.5-vc14_vc15/opencv/build/x64/vc15/lib/")

set(OPEN_CV_INCLUDE "D:/install_soft/opencv-3.4.5-vc14_vc15/opencv/build/include")
set(OPEN0_CV_LIB_DIRECTOR "D:/install_soft/opencv-3.4.5-vc14_vc15/opencv/build/x64/vc15/lib")
set(LIB_NAME "opencv_world345")

include_directories(${OPEN_CV_INCLUDE})
link_directories(${LIB_NAME})

target_link_libraries(${TEST_CHARACTER} characetersegmentaion LIB_NAME)

