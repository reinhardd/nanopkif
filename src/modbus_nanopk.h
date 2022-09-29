#if !defined(__modbus_nanopk_h)
#define __modbus_nanopk_h
#pragma once

#include <chrono>
#include <map>
#include <string>
#include <memory>
#include <cstdint>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <array>
#include <variant>

#include <modbus/modbus-tcp.h>

#include "dataentry.h"
#include "nanopk_visualizer.h"

namespace nanopk 
{

class modbus
{
    using timepoint = std::chrono::steady_clock::time_point;
    using dataentries = std::map<timepoint, dataentry>;
    std::string                 _device;
    std::uint16_t               _port;
    
    std::unique_ptr<modbus_t,  void(*)(modbus_t *)>   
                                _modbus;
                                
    dataentries                 _data;
    
    std::thread                 _thrd;
    std::mutex                  _mtx;
    std::condition_variable     _cnd;
    bool                        _term;
    visualizer                   *_observer;
public:
    modbus(visualizer *, std::string &&dev, std::uint16_t port = 502);
    ~modbus();
    bool start();    

private:
    void process_loop();
    void load_entries();
    void addentry(dataentry &&de, timepoint tp);
    void addentry(const dataentry &de, timepoint tp);
    
    using readresult = queryresult;
    
    readresult process_entry(dataentries::iterator &fe);

    bool wait_for_exit(std::chrono::steady_clock::duration d);
};

}
#endif
 
