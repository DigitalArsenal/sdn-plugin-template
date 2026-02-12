cmake_minimum_required(VERSION 3.16)
project(sdn_plugin C CXX)

# SDN WASI Plugin — CMake build configuration
#
# This builds your plugin as a WASI module for the SDN server (Wazero runtime).
#
# Usage:
#   mkdir build-wasi && cd build-wasi
#   cmake .. -DCMAKE_TOOLCHAIN_FILE=../cmake/wasi-sdk.cmake -DCMAKE_BUILD_TYPE=Release
#   make -j$(nproc)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------- WASI compatibility flags ----------
# Uncomment these if your C++ dependencies need exceptions, RTTI, or signal
# emulation (e.g. Crypto++). For pure C plugins, leave them commented out.

# string(REPLACE "-fno-exceptions" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
# string(REPLACE "-fno-rtti" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -frtti -D_WASI_EMULATED_SIGNAL")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_WASI_EMULATED_SIGNAL")

# Stub headers for WASI (mutex, setjmp, signal, cxa exceptions).
# Added as SYSTEM includes so they shadow missing system headers.
set(WASI_STUBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/wasi-stubs")
include_directories(BEFORE SYSTEM ${WASI_STUBS_DIR})

# ---------- Dependencies ----------
# Add your FetchContent or find_package calls here. Example for Crypto++:
#
# include(FetchContent)
# FetchContent_Declare(cryptopp_cmake
#   GIT_REPOSITORY https://github.com/abdes/cryptopp-cmake.git
#   GIT_TAG        604d3df147e7b16fb7caa70e22c54c9a40ac1bd5
#   GIT_SHALLOW    TRUE)
# set(CRYPTOPP_BUILD_TESTING OFF CACHE BOOL "" FORCE)
# set(CRYPTOPP_INSTALL OFF CACHE BOOL "" FORCE)
# set(CRYPTOPP_DISABLE_ASM ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(cryptopp_cmake)

# ---------- Optimization ----------
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

# ---------- Plugin sources ----------
# Change this to point at your implementation file(s).
# For C++ plugins that use exceptions, also add cxa_stubs.cpp:
#   ${CMAKE_CURRENT_SOURCE_DIR}/src/wasi-stubs/cxa_stubs.cpp
set(PLUGIN_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/plugin.c
)

# ---------- WASI exports ----------
# These are the functions the host runtime (Wazero) will call.
# Do not remove any — they are all required by the SDN plugin ABI.
set(WASI_EXPORTS
  plugin_init
  plugin_get_public_key
  plugin_get_metadata
  plugin_handle_request
)

set(WASI_EXPORT_FLAGS "")
foreach(func ${WASI_EXPORTS})
  list(APPEND WASI_EXPORT_FLAGS "-Wl,--export=${func}")
endforeach()

add_executable(sdn_plugin ${PLUGIN_SOURCES})

target_include_directories(sdn_plugin PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# If using Crypto++ or other libraries, link them here:
# target_link_libraries(sdn_plugin PRIVATE cryptopp)

# ---------- WASI linker flags ----------
# -mexec-model=reactor  = library mode (no main(), exports only)
# malloc/free must be exported so the host can allocate in plugin memory
target_link_options(sdn_plugin PRIVATE
  -mexec-model=reactor
  -Wl,--export=malloc
  -Wl,--export=free
  ${WASI_EXPORT_FLAGS}
)

# Uncomment if using -D_WASI_EMULATED_SIGNAL:
# target_link_options(sdn_plugin PRIVATE -lwasi-emulated-signal)

set_target_properties(sdn_plugin PROPERTIES
  OUTPUT_NAME "my-sdn-plugin"
  SUFFIX ".wasm"
)

message(STATUS "Building SDN Plugin (WASI) for Go/Wazero integration")
message(STATUS "Exported functions: ${WASI_EXPORTS}")
