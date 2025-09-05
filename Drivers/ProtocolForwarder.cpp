#include "ProtocolForwarder.h"

void ProtocolForwarder::addTarget(const ForwardTarget& target)
{
	targets_.push_back(target);
}

void ProtocolForwarder::forward(const ParseResult& result)
{
    for (const auto& target : targets_) {
        if (target.protocol == result.protocol) {
            sendData(target, result.raw);
        }
    }
}

void ProtocolForwarder::sendData(const ForwardTarget& target, const std::vector<uint8_t>& data)
{
    // 实现具体网络发送
    // 使用连接池管理socket连接
}
