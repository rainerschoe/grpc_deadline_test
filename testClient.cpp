// Copyright 2019 IBM Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>

#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>
#include "examples.grpc.pb.h"
#include <chrono>

int main(int argc, char **argv)
{
    examples::StatusHandling::Stub stub(grpc::CreateChannel("127.0.0.1:50051", grpc::InsecureChannelCredentials()));
    grpc::ClientContext context;
    context.set_deadline(std::chrono::system_clock::now() + std::chrono::milliseconds(2000));
    examples::Uint32 request;
    ::google::protobuf::Empty reply;
    request.set_number(5);
    grpc::Status status = stub.rpcSleepForSeconds(&context, request, &reply);

    if (status.ok()) {
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "RPC failed: " << status.error_code() << ": " << status.error_message() << std::endl;
    }
    return 0;
}
