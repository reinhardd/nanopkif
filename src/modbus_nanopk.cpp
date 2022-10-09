
#include <cassert>
#include <functional>
#include <iostream>
#include <iomanip>

#include "modbus_nanopk.h"
#include "nanopk_observer.h"

namespace {
    auto modbus_t_deleter = [](modbus_t *v) { modbus_free(v); };
}    

namespace nanopk
{
    
modbus::modbus(observer *obs, std::string &&dev, std::uint16_t port)
    : _device(dev)
    , _port(port)
    , _modbus(nullptr, modbus_t_deleter)
    , _term(false)
    , _observer(obs)
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

modbus::~modbus()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    {
        std::unique_lock<std::mutex> lck(_mtx);
        _term = true;
    }
    _cnd.notify_one();
    _thrd.join();
}

bool modbus::start()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    modbus_t *tmp = modbus_new_tcp(_device.c_str(), _port);
    if (tmp == nullptr)
    {
        _modbus.reset();
        return false;
    }
    _modbus = std::unique_ptr<modbus_t, void(*)(modbus_t *)> 
                    (tmp, modbus_t_deleter);    
    
    load_entries();
    if (!_data.size())
    {
        _modbus.reset();
        return false;
    }
    
    modbus_connect(_modbus.get());
    modbus_set_slave(_modbus.get(), 1); // ?? do we need it
    modbus_set_response_timeout(_modbus.get(), 1, 0);   // 1 sec
    
    _thrd = std::thread(std::bind(&modbus::process_loop, this));
    return true;
}

void modbus::addentry(const dataentry &de, timepoint tp)
{
    // std::cout << __PRETTY_FUNCTION__ << std::endl;
    while (true)
    {
        auto res = _data.emplace(std::make_pair(tp, de));
        if (res.second)
            return;
        tp += std::chrono::milliseconds(1);
    }
}

void modbus::addentry(dataentry &&de, timepoint tp)
{
    // std::cout << __PRETTY_FUNCTION__ << std::endl;
    // timepoint tp = std::chrono::steady_clock::now();
    while (true)
    {
        auto res = _data.emplace(std::make_pair(tp, std::move(de)));
        if (res.second)
            return;
        tp += std::chrono::milliseconds(1);
    }
}

void modbus::load_entries()
{
    addentry(dataentry("actualSwitchUnit", 0, modbus_data_type::Float,
                            functiontype::ReadHoldingRegisters,
                            seconds(240)), std::chrono::steady_clock::now());

    addentry(dataentry("setSwitchUnit", 2, modbus_data_type::Float,
                            functiontype::ReadHoldingRegisters,
                            seconds(120)), std::chrono::steady_clock::now());

    addentry(dataentry("outdoorTemp", 8, modbus_data_type::Float,
                           functiontype::ReadHoldingRegisters, 
                           seconds(30)), std::chrono::steady_clock::now());
    addentry(dataentry("outdoorAverageTemp", 6, modbus_data_type::Float, 
                           functiontype::ReadHoldingRegisters, 
                           seconds(60)), std::chrono::steady_clock::now());

    addentry(dataentry("boilerTemp1", 10, modbus_data_type::Float,
                           functiontype::ReadHoldingRegisters,
                           seconds(60)), std::chrono::steady_clock::now());
    addentry(dataentry("boilerTemp2", 12, modbus_data_type::Float,
                           functiontype::ReadHoldingRegisters,
                           seconds(60)), std::chrono::steady_clock::now());
    addentry(dataentry("boilerTemp3", 14, modbus_data_type::Float,
                           functiontype::ReadHoldingRegisters,
                           seconds(60)), std::chrono::steady_clock::now());
    addentry(dataentry("boilerTempA", 16, modbus_data_type::Float,
                           functiontype::ReadHoldingRegisters,
                           seconds(60)), std::chrono::steady_clock::now());

    addentry(dataentry("stoveTemp", 82, modbus_data_type::Float,
                           functiontype::ReadHoldingRegisters,
                           seconds(60)), std::chrono::steady_clock::now());
    addentry(dataentry("stoveSetTemp", 84, modbus_data_type::Float,
                           functiontype::ReadHoldingRegisters,
                           seconds(60)), std::chrono::steady_clock::now());

    addentry(dataentry("bufferTopTemp", 122, modbus_data_type::Float,
                           functiontype::ReadHoldingRegisters,
                           seconds(60)), std::chrono::steady_clock::now());
    addentry(dataentry("bufferBottomTemp", 124, modbus_data_type::Float,
                           functiontype::ReadHoldingRegisters,
                           seconds(60)), std::chrono::steady_clock::now());

    addentry(dataentry("exhaustTemp", 126, modbus_data_type::Float,
                           functiontype::ReadHoldingRegisters,
                           seconds(60)), std::chrono::steady_clock::now());


    addentry(dataentry("burnerState", 86, modbus_data_type::Float, 
                           functiontype::ReadHoldingRegisters, 
                           seconds(10)), std::chrono::steady_clock::now());
    addentry(dataentry("boilerState", 178, modbus_data_type::Float, 
                            functiontype::ReadHoldingRegisters, 
                            seconds(10)), std::chrono::steady_clock::now());
    addentry(dataentry("actualFlowTemp", 144, modbus_data_type::Float, 
                            functiontype::ReadHoldingRegisters, 
                            seconds(10)), std::chrono::steady_clock::now());       
    addentry(dataentry("setFlowTemp", 146, modbus_data_type::Float, 
                            functiontype::ReadHoldingRegisters, 
                            seconds(10)), std::chrono::steady_clock::now());



    addentry(dataentry("digital 1001", 1000, modbus_data_type::Integer, 
                            functiontype::ReadHoldingRegisters, 
                            seconds(10)), std::chrono::steady_clock::now());
    addentry(dataentry("digital 1002", 1001, modbus_data_type::Integer, 
                            functiontype::ReadHoldingRegisters, 
                            seconds(10)), std::chrono::steady_clock::now());
    addentry(dataentry("digital 1003", 1002, modbus_data_type::Integer, 
                            functiontype::ReadHoldingRegisters, 
                            seconds(10)), std::chrono::steady_clock::now());
    addentry(dataentry("digital 1004", 1003, modbus_data_type::Integer, 
                            functiontype::ReadHoldingRegisters, 
                            seconds(10)), std::chrono::steady_clock::now());
    addentry(dataentry("digital 1005", 1004, modbus_data_type::Integer, 
                            functiontype::ReadHoldingRegisters, 
                            seconds(10)), std::chrono::steady_clock::now());
    addentry(dataentry("digital 1006", 1005, modbus_data_type::Integer, 
                            functiontype::ReadHoldingRegisters, 
                            seconds(10)), std::chrono::steady_clock::now());
    addentry(dataentry("digital 1007", 1006, modbus_data_type::Integer, 
                            functiontype::ReadHoldingRegisters, 
                            seconds(10)), std::chrono::steady_clock::now());

    addentry(dataentry("B1DayCLK", 2000, modbus_data_type::dayschedule,
                            functiontype::ReadHoldingRegisters,
                            seconds(120)), std::chrono::steady_clock::now());
    addentry(dataentry("B1WCLK1a", 2004, modbus_data_type::weekschedule,
                            functiontype::ReadHoldingRegisters,
                            seconds(120)), std::chrono::steady_clock::now());
    addentry(dataentry("B1WCLK1b", 2009, modbus_data_type::weekschedule,
                            functiontype::ReadHoldingRegisters,
                            seconds(120)), std::chrono::steady_clock::now());
}

bool modbus::wait_for_exit(std::chrono::steady_clock::duration d)
{
    std::unique_lock<std::mutex> lck(_mtx);
    return _cnd.wait_for(lck, d, [this]{ return _term;});
}

dayschedule frombytes(uint16_t *data)
{
    dayschedule ds;
    ds.s1.first = data[0];
    ds.s1.second = data[2];
    ds.s2.first = data[1];
    ds.s2.second = data[3];
    return ds;
}

modbus::readresult modbus::process_entry(dataentries::iterator &fe)
{    
    // std::cout << __PRETTY_FUNCTION__ << std::endl;
    entrydata readval;
    switch (fe->second.datatype)
    {
        case modbus_data_type::Selection:
        case modbus_data_type::Integer:
            {
                uint16_t data;
                int r = modbus_read_registers(_modbus.get(), fe->second.readaddr, 1, &data);
                if (r == -1) 
                {
                    std::cout << "Integer read failed res " << r << std::endl;
                    // fe->second.result = std::monostate();
                    return readresult::invalid_response;
                }       
                if (r != 1) 
                {
                    std::cout << "Integer read failed for addr " 
                              << fe->second.readaddr 
                              << " reported wrong response length " << r
                              << " expected 1"
                              << std::endl;
                    return readresult::unexpected_size;
                }
#if defined(TRACE_VARS)
                std::cout << "result for  " << fe->second.varname << " (" 
                          << fe->second.readaddr << "): 0x"
                          << std::hex << std::setfill('0') << std::setw(4) 
                          << data 
                          << std::setfill(' ')
                          << std::dec << std::endl;
#endif
                readval = data;
            }
            break;
        case modbus_data_type::Float:
            {
                union {
                    uint16_t raw[2];   // big endian for register and order
                    float f;                    
                } data;
                // std::cout << "try float read from " << fe->second.readaddr << std::endl;
                int r = modbus_read_registers(_modbus.get(), 
                                              fe->second.readaddr, 2, 
                                              data.raw);
                
                std::swap(data.raw[0], data.raw[1]);
                // std::cout << "read res " << r << std::endl;
                if (r == -1) 
                {
                    std::cout << "float read failed for addr " 
                              << fe->second.readaddr << std::endl;                    
                    return readresult::invalid_response;
                }                
                if (r != 2) 
                {
                    std::cout << "float read failed for addr " 
                              << fe->second.readaddr 
                              << " reported wrong response length " << r
                              << " expected 2"
                              << std::endl;
                    return readresult::unexpected_size;
                }                
#if defined(TRACE_VARS)
                std::cout << "result for  " << fe->second.varname << " (" 
                          << fe->second.readaddr << "): "
                          << data.f << std::endl;
#endif
                readval = data.f;
            }
            break;
        case modbus_data_type::dayschedule:
            {
                uint16_t data[4];
                int r = modbus_read_registers(_modbus.get(), fe->second.readaddr, 4, data);
                if (r == -1)
                {
                    std::cout << "dayschedule read failed for addr "
                              << fe->second.readaddr << std::endl;
                    return readresult::invalid_response;
                }
                if (r != 4)
                {
                    std::cout << "dayschedule read failed for addr "
                              << fe->second.readaddr
                              << " reported wrong response length " << r
                              << " expected 4"
                              << std::endl;
                    return readresult::unexpected_size;
                }
                readval = frombytes(data);
            }
            break;
        case modbus_data_type::weekschedule:
            {
                uint16_t data[5];
                int r = modbus_read_registers(_modbus.get(), fe->second.readaddr, 5, data);
                if (r == -1)
                {
                    std::cout << "weekdayschedule read failed for addr "
                              << fe->second.readaddr << std::endl;
                    return readresult::invalid_response;
                }
                if (r != 5)
                {
                    std::cout << "weekdayschedule read failed for addr "
                              << fe->second.readaddr
                              << " reported wrong response length " << r
                              << " expected 4"
                              << std::endl;
                    return readresult::unexpected_size;
                }
                weekdayschedule wds;
                wds.day = static_cast<daydefinition>(data[0]);
                wds.schedule = frombytes(&data[1]);
                readval = wds;
            }
            break;
        default:
            break;
    }
    bool unchanged = (fe->second.result == readval);
    if (!unchanged)
        fe->second.result = readval;
    fe->second.tstamp = std::chrono::system_clock::now();
    return unchanged ? readresult::valid_unchanged : readresult::valid;
}

void modbus::process_loop()
{
    timepoint last_transmit;
    while (true)
    {        
        auto now = std::chrono::steady_clock::now();
        dataentries::iterator fe = _data.begin();
        auto diff = fe->first - now;
        if (diff < std::chrono::microseconds(0))
        {
            readresult r = process_entry(fe);
            switch (r)
            {
                case readresult::invalid_response:
                    std::cerr << "invalid response\n";
                    break;
                case readresult::timeout:
                    std::cerr << "timeout\n";
                    break;
                case readresult::unexpected_size:
                    std::cerr << "internal error\n";
                    throw std::runtime_error("internal error: wrong response size");
                    break;
                case readresult::valid:
                    // std::cout << "changed\n";
                    {
                        eventdata d;
                        d.name = fe->second.varname;
                        d.data = fe->second.result;
                        d.tstamp = fe->second.tstamp;
                        _observer->on_change(d);
                    }
                    break;
                case readresult::valid_unchanged:
                    // std::cout << "unchanged\n";
                    break;

            }

            last_transmit = std::chrono::steady_clock::now();
            auto ntime = last_transmit + fe->second.pollintervall;
            addentry(fe->second, ntime);
            _data.erase(fe->first);
            if (wait_for_exit(std::chrono::milliseconds(200)))
                return;
        }
        else
        {
#if defined(TRACE_TIMING)
            std::cout << "sleep for "
                      << std::chrono::duration_cast<std::chrono::seconds>(diff).count() 
                      << " seconds\n";
#endif
            if (wait_for_exit(diff))
            {
                std::cout << "exit " << __PRETTY_FUNCTION__ << std::endl;
                return;
            }
        }
    }
}
}
