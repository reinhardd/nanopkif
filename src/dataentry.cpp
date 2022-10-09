
#include <sstream>
#include <iomanip>
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

std::ostream &operator<<(std::ostream &s, const std::monostate &)
{
    s << "<empty>";
    return s;
}

std::string showtime(uint16_t d)
{
    std::ostringstream xs;
    xs << std::setw(2) << std::setfill('0')
       << (d/60) << ':' << std::setw(2) << (d % 60);
    return xs.str();
}

std::ostream &operator<<(std::ostream &s, const dayschedule &d)
{
    std::ostringstream xs;
    xs << "1: " << showtime(d.s1.first) << '-' << showtime(d.s1.second)
       << " 2: " << showtime(d.s2.first) << '-' << showtime(d.s2.second);
    s << xs.str();
    return s;
}
std::ostream &operator<<(std::ostream &s, const weekdayschedule &w)
{
    std::ostringstream xs;
    uint8_t tmp = uint8_t(w.day);
    if (tmp & nanopk::daydefinition::sunday)
        xs << "sun;";
    if (tmp & nanopk::daydefinition::monday)
        xs << "mon;";
    if (tmp & nanopk::daydefinition::tuesday)
        xs << "tue;";
    if (tmp & nanopk::daydefinition::wednesday)
        xs << "wed;";
    if (tmp & nanopk::daydefinition::thursday)
        xs << "thu;";
    if (tmp & nanopk::daydefinition::friday)
        xs << "fri;";
    if (tmp & nanopk::daydefinition::saturday)
        xs << "sat;";
    xs << " " << w.schedule;
    s << xs.str();
    return s;
}

} // nanopk
