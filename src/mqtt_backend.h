#ifndef MQTT_BACKEND_H
#define MQTT_BACKEND_H
#pragma once

#include <cstdint>
#include <string>
#include <thread>


#include "nanopk_observer.h"

namespace nanopk
{

class mqtt_backend_impl;

class mqtt_backend : public observer
{
public:
    mqtt_backend(const std::string &host, uint16_t port);
    void disconnected() override;
    void update() override;
    void on_change(const eventdata &d) override;

private:
    struct Private;
    std::unique_ptr<mqtt_backend_impl> _impl;
};
}
#endif // MQTT_BACKEND_H
