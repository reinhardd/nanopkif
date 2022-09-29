
#include "dataentry.h"

namespace nanopk 
{

dataentry::dataentry(std::string &&basename, 
                     uint16_t addr, 
                     modbus_data_type t, 
                     functiontype f, 
                     seconds i)
    : varname(basename)
    , readaddr(addr)
    , datatype(t)
    , readtype(f)
    , pollintervall(i)
{
}

bool operator==(const dayschedule &lhs, const dayschedule &rhs)
{
    return lhs.s1 == rhs.s1 && lhs.s2 == rhs.s2;
}
bool operator==(const weekdayschedule &lhs, const weekdayschedule &rhs)
{
    return lhs.day == rhs.day && lhs.schedule == rhs.schedule;
}

} // nanopk
