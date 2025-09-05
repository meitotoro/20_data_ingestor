#pragma once
#include <string>
#include <CommonTypes.h>
#include <vector>
#include <unordered_map>
#include <memory>



class ProtocolForwarder
{
public: 
    struct ForwardTarget {
        std::string address;
        uint16_t port;
        ProtocolType protocol;
    };
    void addTarget(const ForwardTarget& target);
    void forward(const ParseResult& result);
private:
    void sendData(const ForwardTarget& target, const std::vector<uint8_t>& data);

    std::vector<ForwardTarget> targets_;
   // std::unordered_map<std::string, std::shared_ptr<SocketConnection>> connections_;
};

