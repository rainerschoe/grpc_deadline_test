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

class ServiceStatusHandling final : public examples::StatusHandling::Service
{
    virtual ::grpc::Status rpcSleepForSeconds(
           ::grpc::ServerContext* context,
           const examples::Uint32* request,
           ::google::protobuf::Empty* response
           ) override
    {
        int sleepTime = request->number();
        sleep(sleepTime);

        return grpc::Status();
    }
};

int main(int argc, char **argv)
{
    grpc::EnableDefaultHealthCheckService(true);

    grpc::ServerBuilder builder;
    std::string insecureServerAddr = "[::]:50051";
    builder.AddListeningPort(insecureServerAddr, grpc::InsecureServerCredentials());

    ServiceStatusHandling statusHandling;
    builder.RegisterService(&statusHandling);

    std::cout << "Starting insecure server listening on " << insecureServerAddr << std::endl;
    std::unique_ptr<grpc::Server> server(builder.BuildAndStart());

    if (server != nullptr)
    {
        server->Wait();
    }
    else
    {
        std::cout << "Server failed to start. exiting." << std::endl;
        return -1;
    }
    return 0;
}
