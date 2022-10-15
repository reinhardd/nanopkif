#ifndef CONSOLE_INFO_H
#define CONSOLE_INFO_H
#pragma once

#include <string>
#include <map>

#include "nanopk_observer.h"

namespace nanopk
{

class console_info : public observer
{
    std::map<std::string, eventdata> _evd;
public:
    void disconnected() override {}
    void update() override {}
    void on_change(const eventdata &d) override;

private:
    std::string getValue(const std::string &key);
};

}

#endif // CONSOLE_INFO_H
