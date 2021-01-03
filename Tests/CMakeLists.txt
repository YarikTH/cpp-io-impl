cmake_minimum_required(VERSION 3.12)

# ============================ FileBenchmark ==================================

add_executable(FileBenchmark
	FileBenchmark.cpp)

target_link_libraries(FileBenchmark PRIVATE Library)
