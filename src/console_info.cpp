
#include <iostream>
#include <sstream>


#include "console_info.h"
#include "dataentry.h"

namespace nanopk
{

void console_info::on_change(const eventdata &d)
{
    _evd[d.name] = d;

    std::ostringstream xs;
    xs << "=== environment ===\n"
       << "outdoor temp [°C] "
       << getValue("outdoorTemp")
       << " avg: " << getValue("outdoorAverageTemp")
       << "\n";
    xs << "=== stove ===\n"
       << "state: " << getValue("burnerState")
       << "\ntemp [°C] set:" << getValue("stoveSetTemp")
       << " act:" << getValue("stoveTemp")
       << "\n exhaust: " << getValue("exhaustTemp")
       << "\n";
    xs << "=== heating circuit ===\n"
       << "flow setpoint [°C] " << getValue("setFlowTemp")
       << " current " << getValue("actualFlowTemp");
    xs << "\n=== boiler ===\n"
       << "temp [°C] 1: "
       << getValue("boilerTemp1") << '\n';
    xs << "\n=== buffer ===\n"
       << "[°C] top:" << getValue("bufferTopTemp")
       << " bottom:" << getValue("bufferBottomTemp");

    xs << "\n=== schedules ===\n"
       << "B1 " << getValue("B1DayCLK")
       << "\nB1a " << getValue("B1WCLK1a")
       << "\nB1b " << getValue("B1WCLK1b");
    std::cout << xs.str() << std::endl;
}

std::string console_info::getValue(const std::string &key)
{
    auto cit = _evd.find(key);
    if (cit == _evd.end())
    {
        return "-";
    }
    std::cout << "\n";

    std::ostringstream os;
    std::visit([&os](const auto &x) { os << x; }, cit->second.data);
    return os.str();
}

}
