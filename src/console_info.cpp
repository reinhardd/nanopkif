
#include <iostream>
#include <sstream>


#include "console_info.h"
#include "dataentry.h"

namespace nanopk
{

std::string stoveState2String(unsigned u);

void console_info::on_change(const eventdata &d)
{
    _evd[d.name] = d;

    std::string stoveState = getValue("burnerState");
    std::istringstream ins(stoveState);
    unsigned uStoveState;
    ins >> uStoveState;

    std::ostringstream xs;
    xs << "=== environment ===\n"
       << "outdoor temp [°C]"
       << "\n\tact: " << getValue("outdoorTemp")
       << "\n\tavg: " << getValue("outdoorAverageTemp")
       << "\n";
    xs << "=== stove ===\n"
       << "state: " << stoveState << ':' << stoveState2String(uStoveState)
       << "\ntemp [°C]\n\tset: " << getValue("stoveSetTemp")
       << "\n\tact: " << getValue("stoveTemp")
       << "\n\texhaust: " << getValue("exhaustTemp")
       << "\n";
    xs << "=== heating circuit ===\n"
       << "flow [°C]\n\tset: " << getValue("setFlowTempHK1")
       << "\n\tact: " << getValue("actualFlowTempHK1")
       << "\nroom set temp [°C]"
       << "\n\t  day: " << getValue("heatingDayRoomTemp1")
       << "\n\tnight: " << getValue("heatingNightRoomTemp1")
       << "\nschedules "
       << "\n\tday " << getValue("heatingDCLK1")
       << "\n\t  a " << getValue("heatingWCLK1a")
       << "\n\t  b " << getValue("heatingWCLK1b")
       << "\n\t  c " << getValue("heatingWCLK1c")
       << "\n\t  d " << getValue("heatingWCLK1d")
       << "\n\t  e " << getValue("heatingWCLK1e")
       << "\n\t  f " << getValue("heatingWCLK1f")
       << "\n\t  g " << getValue("heatingWCLK1g")
       << '\n';
    xs << "\n=== boiler ==="
       << "\nstate: " << getValue("boilerState")
       << "temp [°C]"
       << "\n\tset: " << getValue("setTempBoiler1")
       << "\n\tact: " << getValue("boilerTemp1")
       << "\nschedules"
       << "\n\tcirculationPump " << getValue("circulationPumpDCLK")
       << "\n\tday "  << getValue("boiler1DayCLK")
       << "\n\t  a "  << getValue("boiler1WCLKa")
       << "\n\t  b "  << getValue("boiler1WCLKb")
       << "\n\t  c "  << getValue("boiler1WCLKc")
       << "\n\t  d "  << getValue("boiler1WCLKd")
       << "\n\t  e "  << getValue("boiler1WCLKe")
       << "\n\t  f "  << getValue("boiler1WCLKf")
       << "\n\t  g "  << getValue("boiler1WCLKg") << '\n';
    xs << "\n=== buffer ===\n"
       << "temp [°C]\n\t   top: " << getValue("bufferTopTemp")
       << "\n\tbottom: " << getValue("bufferBottomTemp");
    xs << "\n=== conveying ==="
       << "\n\tpos: " << getValue("actualSwitchUnit")
       << "\n\tset: " << getValue("setSwitchUnit");

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

std::string stoveState2String(unsigned u)
{
    switch (u)
    {
        case 0: return "init";
        case 1: return "aus";
        case 2: return "Schieberost init";
        case 3: return "start";
        case 4: return "Zuendung Einschub";
        case 5: return "Zuendung Pause";
        case 6: return "Zuendung reduziert";
        case 7: return "Leistungsbrand";
        case 8: return "Gluterhaltung";
        case 9: return "Entaschung";
        case 10: return "Entaschung Rost";
        case 11: return "Entaschung Nachlauf";
        case 12: return "Reinigung";
        case 13: return "Fuellen Start";
        case 14: return "Fuellen";
        default: return "-";
    }
    return "-";
}

}
