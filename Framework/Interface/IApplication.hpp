#pragma once
#include "Interface.hpp"
#include "IRuntimeModule.hpp"

namespace OctoPotato {
    Interface IApplication : implements IRuntimeModule {
    public:
        virtual bool isQuit() = 0;
    };
}