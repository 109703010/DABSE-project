cmake_minimum_required(VERSION 3.10)
project(DABSE)
set(CMAKE_CXX_STANDARD 14)
# set(CMAKE_OSX_ARCHITECTURES  "arm64" CACHE STRING "Build architectures for Mac OS X" FORCE)

# header include path
set(LOCAL_H /usr/local/include)
include_directories(${LOCAL_H})

# link library
find_file(GMP_LINK NAMES libgmp.a HINTS /usr/local/lib)
find_file(CRY_LINK NAMES libcrypto.a HINTS /usr/local/lib)
find_file(PBC_LINK NAMES libpbc.a HINTS /usr/local/lib)
find_file(SSL_LINK NAMES libssl.a HINTS /usr/local/lib)
link_libraries(${GMP_LINK} ${CRY_LINK} ${PBC_LINK} ${SSL_LINK})

# add executable
set(OBJ_PATH policy/data_structure/)
add_executable(main main.c ${OBJ_PATH}access_tree/access_tree.c ${OBJ_PATH}access_tree/AT_stack.c ${OBJ_PATH}access_tree/AT_queue.c ${OBJ_PATH}char/char_stack.c ${OBJ_PATH}int/int_vector.c)
