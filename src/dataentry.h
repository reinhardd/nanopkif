#if !defined(__dataentries_h)
#define __dataentries_h
#pragma once

#include <cstdint>
#include <chrono>
#include <variant>
#include <utility>
#include <string>
	
#include <modbus/modbus.h>

namespace nanopk 
{

enum struct functiontype 
{
    ReadCoils               = MODBUS_FC_READ_COILS,
    ReadDiscreteInputs      = MODBUS_FC_READ_DISCRETE_INPUTS,
    ReadHoldingRegisters    = MODBUS_FC_READ_HOLDING_REGISTERS,
    ReadInputRegisters      = MODBUS_FC_READ_INPUT_REGISTERS,
    WriteSingleCoil         = MODBUS_FC_WRITE_SINGLE_COIL,
    WriteSingleRegister     = MODBUS_FC_WRITE_SINGLE_REGISTER,
    WriteMultipleCoils      = MODBUS_FC_WRITE_MULTIPLE_COILS,
    WriteMultipleRegisters  = MODBUS_FC_WRITE_MULTIPLE_REGISTERS,
};

enum struct modbus_data_type
{
    Integer, 
    Float,
    Selection,
    dayschedule,
    weekschedule,
};

using seconds = std::chrono::seconds;
using timestamp = std::chrono::system_clock::time_point;

enum daydefinition:uint8_t
{
    sunday          = (1 << 0),
    monday          = (1 << 1),
    tuesday         = (1 << 2),
    wednesday       = (1 << 3),
    friday          = (1 << 4),
    saturday        = (1 << 5)
};

using startstopdef = std::pair<uint16_t, uint16_t>;

struct dayschedule
{
    startstopdef s1, s2;
};

struct weekdayschedule
{
    daydefinition day;
    dayschedule schedule;
};

using entrydata = std::variant<
        std::monostate,
        uint16_t,                  // Integer + Auswahl
        float,                     // Float
        dayschedule,               // day schedule
        weekdayschedule            // week schedule
  >;

  
bool operator==(const dayschedule &lhs, const dayschedule &rhs);
bool operator==(const weekdayschedule &lhs, const weekdayschedule &rhs);

enum struct queryresult
{
    valid,
    valid_unchanged,
    invalid_response,
    unexpected_size,
    timeout,
};


struct dataentry
{
    std::string varname;
    uint16_t readaddr;
    modbus_data_type datatype;
    functiontype readtype;
    
    // define readentries later if needed
    seconds pollintervall;  
    
    entrydata  result;
    timestamp    tstamp;
    
    dataentry(std::string &&basename, 
              uint16_t addr, 
              modbus_data_type t, 
              functiontype f, 
              seconds i);
    // bool operator<(const dataentry &rhs) const;
};

} // nanopk
#endif
