cmake_minimum_required(VERSION 2.8)

project( Histogram )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

#由 .cpp file 建立執行檔
add_executable( Histogram Histogram.cpp )

#將 cv library 與執行檔連結
target_link_libraries( Histogram ${OpenCV_LIBS} )
