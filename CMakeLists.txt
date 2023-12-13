cmake_minimum_required(VERSION 3.10)
project(test1)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_OSX_ARCHITECTURES  "arm64" CACHE STRING "Build architectures for Mac OS X" FORCE)

# header include path
set(LOCAL_H /usr/local/include)
include_directories(${LOCAL_H})

# add libraries
set(GMP_LINK /usr/local/lib/libgmp.a)
set(CRY_LINK /usr/local/lib/libcrypto.a)
set(PBC_LINK /usr/local/lib/libpbc.a)
set(SSL_LINK /usr/local/lib/libssl.a)
link_libraries(${GMP_LINK} ${CRY_LINK} ${PBC_LINK} ${SSL_LINK})

set(OBJ_PATH policy/obj/data_structure/)
add_executable(main main.c ${OBJ_PATH}access_tree/access_tree.o ${OBJ_PATH}access_tree/AT_stack.o ${OBJ_PATH}access_tree/AT_queue.o ${OBJ_PATH}char/char_stack.o ${OBJ_PATH}int/int_vector.o)
