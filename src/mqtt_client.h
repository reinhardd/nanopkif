#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H
#pragma once

#include <mqtt_client_cpp.hpp>

class mqtt_client
{
    std::thread iocthrd;
    boost::asio::io_context ioc;

    mqtt_client(const std::string &host, uint16_t port)
    {

    }
};


#endif // MQTT_CLIENT_H
