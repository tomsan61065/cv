cmake_minimum_required(VERSION 2.8)

project( midterm )

find_package( OpenCV REQUIRED )

include_directories( ${OpenCV_INCLUDE_DIRS} )

#由 .cpp file 建立執行檔
add_executable( midterm midterm.cpp )

#將 cv library 與執行檔連結
target_link_libraries( midterm ${OpenCV_LIBS} )
