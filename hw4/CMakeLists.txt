cmake_minimum_required(VERSION 2.8)

project( HarrisCorner )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_CXX_STANDARD 11) # 啟用 c++ 11
#由 .cpp file 建立執行檔
add_executable( HarrisCorner HarrisCorner.cpp )

#將 cv library 與執行檔連結
target_link_libraries( HarrisCorner ${OpenCV_LIBS} )
