# Copyright 2014 Carnegie Mellon University
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 2.6)

# basic configuration
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
	message(FATAL_ERROR "Use out-of-source build only!")
endif(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)

project(MEHCACHED)

add_subdirectory(perf_count)
set(LIBRARY_PATH ${LIBRARY_PATH} perf_count/)

# compiler options
add_definitions(-pthread)
add_definitions(-g -Wall -Wextra -Wsign-conversion -Winline -Wno-unused-function)
add_definitions(-Wconversion)
add_definitions(-O9)
add_definitions(-msse4.2 -march=corei7)

# preprocessor
SET(NDEBUG "$ENV{NDEBUG}" CACHE STRING "define NDEBUG macro")

message(STATUS "NDEBUG (disable all additional checkings; no*, yes) = ${NDEBUG}")
if("${NDEBUG}" STREQUAL "yes")
	add_definitions(-DNDEBUG)
endif()

SET(NLOCAL "$ENV{NLOCAL}" CACHE STRING "no local programs")
SET(NCLIENT "$ENV{NCLIENT}" CACHE STRING "no clients")
SET(NSERVER "$ENV{NSERVER}" CACHE STRING "no servers")

message(STATUS "NLOCAL (do not compile local programs; no*, yes) = ${NLOCAL}")
message(STATUS "NCLIENT (do not compile clients; no*, yes) = ${NCLIENT}")
message(STATUS "NSERVER (do not compile server; no*, yes) = ${NSERVER}")

# common source files
set(SOURCES ${SOURCES} hash.c)
set(SOURCES ${SOURCES} city.c)
set(SOURCES ${SOURCES} stopwatch.c)
set(SOURCES ${SOURCES} shm.c)

# common libraries
set(LIBRARIES ${LIBRARIES} rt crypto perf_count m pthread)

# tcmalloc (optional)
#add_definitions(-fno-builtin-malloc -fno-builtin-calloc -fno-builtin-realloc -fno-builtin-free)
#set(LIBRARIES ${LIBRARIES} tcmalloc_minimal)

# DPDK
set(RTE_SDK ${CMAKE_CURRENT_SOURCE_DIR}/../../DPDK)
set(RTE_TARGET x86_64-native-linuxapp-gcc)
add_definitions(-DUSE_DPDK)
add_definitions(-isystem ${RTE_SDK}/${RTE_TARGET}/include -include rte_config.h)
#add_definitions(-isystem ${RTE_SDK}/include/dpdk -include rte_config.h)
link_directories(${LINK_DIRECTORIES} ${RTE_SDK}/${RTE_TARGET}/lib)
#link_directories(${LINK_DIRECTORIES} ${RTE_SDK}/lib)
set(LIBRARIES ${LIBRARIES} rte_eal rte_mempool rte_ring)	# for basic features
set(LIBRARIES ${LIBRARIES} rte_ethdev rte_mbuf rte_pmd_ixgbe librte_pmd_ring.a)	# for network features
set(LIBRARIES ${LIBRARIES} pthread rt)	# to make sure symbols in pthread and rt are resolved

#if(NOT "${NLOCAL}" STREQUAL "yes")
	# microbench
	add_executable(microbench microbench.c ${SOURCES})
	target_link_libraries(microbench ${LIBRARIES})
	target_link_libraries(microbench ${CMAKE_DL_LIBS})

	# microbench_store
	add_executable(microbench_store microbench.c ${SOURCES})
	set_target_properties(microbench_store PROPERTIES COMPILE_FLAGS "-DMEHCACHED_NO_EVICTION")
	target_link_libraries(microbench_store ${LIBRARIES})
	target_link_libraries(microbench_store ${CMAKE_DL_LIBS})
	#endif()
