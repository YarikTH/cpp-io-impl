cmake_minimum_required(VERSION 3.12)

# ================================ ByteIO =====================================

add_executable(ByteIO
	ByteIO.cpp)

target_link_libraries(ByteIO PRIVATE Library)

# =============================== WriteInt ====================================

add_executable(WriteInt
	WriteInt.cpp)

target_link_libraries(WriteInt PRIVATE Library)

# ============================ WriteIntFixed ==================================

add_executable(WriteIntFixed
	WriteIntFixed.cpp)

target_link_libraries(WriteIntFixed PRIVATE Library)

# ============================= HelloWorld ====================================

add_executable(HelloWorld
	HelloWorld.cpp)

target_link_libraries(HelloWorld PRIVATE Library)
