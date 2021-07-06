cmake_minimum_required(VERSION 3.0.0)
project(hog-video-detection VERSION 0.1.0)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "../bin")
add_compile_definitions(
    _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
)

include(CTest)
enable_testing()

find_package( OpenCV REQUIRED )
find_package( Threads REQUIRED )

# link_libraries(stdc++fs)

add_executable(${PROJECT_NAME}
  "source_files/main.cpp"
  "source_files/hog_video_controller.cpp"
)

target_link_libraries(${PROJECT_NAME} 
  ${OpenCV_LIBS} 
  ${CMAKE_THREAD_LIBS_INIT} 
  stdc++fs
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
