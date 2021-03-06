# Copyright 2019 IBM Corporation
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

cmake_minimum_required (VERSION 3.11)
project (gWhisper)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


################################################################################
# BUILD OPTIONS
################################################################################
option(GWHISPER_FORCE_BUILDING_GRPC "Fetch, build and statically link against gRPC even if gRPC is found to be installed" OFF)

include(gRPC)

################################################################################
# SERVER
################################################################################
set(TARGET_NAME "testServer")
set(TARGET_SRC
    testServer.cpp
    )

add_protobuf_grpc_lib( ${TARGET_NAME}_protobuf PROTO_SOURCES examples.proto)

add_executable(${TARGET_NAME} ${TARGET_SRC} ${PROTO_SRC})

target_link_libraries(${TARGET_NAME}
    PUBLIC
    ${TARGET_NAME}_protobuf

    # we need to link the whole reflection lib, even though the compiler might
    # think we do not use any symbols. (This is because gRPC dynamically
    # checks if reflection is linked and has no hard dependency on any symbols
    # there)
    # This is done with --no-as-needed in case we link dynamically and
    # --whole-archive in case we link statically.
    -Wl,--push-state,--no-as-needed,--whole-archive ${LIB_GRPC++_REFLECTION} -Wl,--pop-state
    )

################################################################################
# CLIENT
################################################################################
set(TARGET_NAME "testClient")
set(TARGET_SRC
    testClient.cpp
    )

add_executable(${TARGET_NAME} ${TARGET_SRC} ${PROTO_SRC})

target_link_libraries(${TARGET_NAME}
    PUBLIC
    testServer_protobuf
    ${LIB_GRPC++}
    )
