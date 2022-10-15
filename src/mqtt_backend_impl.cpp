
#include <iostream>

#include "mqtt_backend_impl.h"

namespace nanopk
{

mqtt_backend_impl::mqtt_backend_impl(const std::string &host, uint16_t port)
{
    _client = MQTT_NS::make_sync_client(_ioc, host, port);
    _client->set_connack_handler(std::bind(&mqtt_backend_impl::handle_connack,
                                           this,
                                           std::placeholders::_1,
                                           std::placeholders::_2));
    _client->set_close_handler(std::bind(&mqtt_backend_impl::handle_close,
                                         this));
    _client->set_error_handler(std::bind(&mqtt_backend_impl::handle_error,
                                         this,
                                         std::placeholders::_1));
    _client->set_puback_handler(std::bind(&mqtt_backend_impl::handle_puback,
                                         this,
                                         std::placeholders::_1));
    _client->set_pubrec_handler(std::bind(&mqtt_backend_impl::handle_pubrec,
                                         this,
                                         std::placeholders::_1));
    _client->set_pubcomp_handler(std::bind(&mqtt_backend_impl::handle_pubcomp,
                                         this,
                                         std::placeholders::_1));
    _client->set_suback_handler(std::bind(&mqtt_backend_impl::handle_suback,
                                         this,
                                         std::placeholders::_1,
                                         std::placeholders::_2));

    _client->set_publish_handler(std::bind(&mqtt_backend_impl::handle_publish,
                                         this,
                                         std::placeholders::_1,
                                         std::placeholders::_2,
                                         std::placeholders::_3,
                                         std::placeholders::_4));
}



bool mqtt_backend_impl::handle_connack(
        bool sp,
        MQTT_NS::connect_return_code connack_return_code)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return true;
}

void mqtt_backend_impl::handle_close()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

void mqtt_backend_impl::handle_error(MQTT_NS::error_code e)
{
    std::cout << "error " << e.message() << std::endl;
}

bool mqtt_backend_impl::handle_puback(packet_id_t id)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return true;
}
bool mqtt_backend_impl::handle_pubrec(packet_id_t id)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return true;
}
bool mqtt_backend_impl::handle_pubcomp(packet_id_t id)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
     return true;
}
bool mqtt_backend_impl::handle_suback(packet_id_t id,
                   std::vector<MQTT_NS::suback_return_code> results)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return true;
}
bool mqtt_backend_impl::handle_publish(MQTT_NS::optional<packet_id_t> packet_id,
                    MQTT_NS::publish_options pubopts,
                    MQTT_NS::buffer topic_name,
                    MQTT_NS::buffer contents)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return true;
}


}


