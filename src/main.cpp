
#include <iostream>
#include <iomanip>
#include <map>
#include <set>

#include "modbus_nanopk.h"
#include "nanopk_observer.h"
#include "console_info.h"

class observer_dispatcher : public nanopk::observer
{
    std::set<nanopk::observer *> _obs;
public:
    observer_dispatcher() = default;
    void add(nanopk::observer *p)
    {
        _obs.insert(p);
    }
    void remove(nanopk::observer *p)
    {
        _obs.erase(p);
    }
    virtual void disconnected()
    {
        for (auto *p: _obs)
            p->disconnected();
    }
    virtual void update()
    {
        for (auto *p: _obs)
            p->update();
    }
    virtual void on_change(const nanopk::eventdata &d)
    {
        for (auto *p: _obs)
            p->on_change(d);
    }
};

int main(int argc, char** argv) 
{
    if (argc != 3) 
    {
        std::cout << argv[0] << " host port" << std::endl;
        return -1;
    }

    observer_dispatcher od;
    od.add(new nanopk::console_info());

    std::cout << "start mbus\n";
    nanopk::modbus modbusmaster(&od, "192.168.0.45", 502);
    modbusmaster.start();
    while (true)
        std::this_thread::sleep_for(std::chrono::seconds(10));
}
