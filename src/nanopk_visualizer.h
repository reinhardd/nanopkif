#if !defined(__nanopk_observer_h)
#define __nanopk_observer_h
#pragma once

namespace nanopk 
{
    
struct eventdata {
    entrydata data;
    timestamp tstamp;
    std::string name;
};    
    
class visualizer
{
public:
    virtual ~visualizer() = default;
    virtual void disconnected() = 0;
    virtual void update() = 0;
    virtual void on_change(const eventdata &d) = 0;
};

}
#endif
