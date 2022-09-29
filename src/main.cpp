
#include <iostream>
#include <iomanip>
#include <map>

#include "modbus_nanopk.h"

int main(int argc, char** argv) 
{
    if (argc != 3) 
    {
        std::cout << argv[0] << " host port" << std::endl;
        return -1;
    }

    std::cout << "start mbus\n";
    nanopk::modbus nanopk_observer(nullptr, "192.168.0.45", 502);
    nanopk_observer.start();
    while (true)
        std::this_thread::sleep_for(std::chrono::seconds(10));
}
