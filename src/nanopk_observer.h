#if !defined(__nanopk_observer_h)
#define __nanopk_observer_h
#pragma once

#include "dataentry.h"

namespace nanopk 
{
    
struct eventdata {
    entrydata data;
    timestamp tstamp;
    std::string name;
};    
    
class observer
{
public:
    virtual ~observer() = default;
    virtual void disconnected() = 0;
    virtual void update() = 0;
    virtual void on_change(const eventdata &d) = 0;
};

}
#endif
