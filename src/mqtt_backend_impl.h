#ifndef MQTT_BACKEND_IMPL_C
#define MQTT_BACKEND_IMPL_C

#include <thread>

#include <mqtt_client_cpp.hpp>

namespace nanopk
{

using sync_client =
      MQTT_NS::callable_overlay<
          MQTT_NS::sync_client<
              MQTT_NS::tcp_endpoint<as::ip::tcp::socket, as::io_context::strand>
          >
      >;

using packet_id_t = sync_client::packet_id_t;

using sync_client_shared = std::shared_ptr<sync_client>;

class mqtt_backend_impl
{
private:
    std::thread _iocthrd;
    boost::asio::io_context _ioc;


    sync_client_shared _client;
public:
    mqtt_backend_impl(const std::string &host, uint16_t port);
private:
    // mqtt handlers
    bool handle_connack(bool sp, MQTT_NS::connect_return_code connack_return_code);
    void handle_close();
    void handle_error(MQTT_NS::error_code);
    bool handle_puback(packet_id_t id);
    bool handle_pubrec(packet_id_t id);
    bool handle_pubcomp(packet_id_t id);
    bool handle_suback(packet_id_t id,
                       std::vector<MQTT_NS::suback_return_code> results);
    bool handle_publish(MQTT_NS::optional<packet_id_t> packet_id,
                        MQTT_NS::publish_options pubopts,
                        MQTT_NS::buffer topic_name,
                        MQTT_NS::buffer contents);



};

}
#endif // MQTT_BACKEND_IMPL_C
