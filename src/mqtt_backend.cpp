#include <iostream>
#include <iomanip>


#include "mqtt_backend.h"
#include "mqtt_backend_impl.h"


namespace nanopk
{


mqtt_backend::mqtt_backend(const std::string &host, uint16_t port)
    : _impl(std::make_unique<mqtt_backend_impl>(host, port))

{
}

void mqtt_backend::disconnected()
{
}

void mqtt_backend::update()
{

}
void mqtt_backend::on_change(const eventdata &d)
{

}


}
