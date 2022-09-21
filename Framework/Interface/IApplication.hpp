#pragma once
#include "Interface.hpp"
#include "IRuntimeModule.hpp"

namespace OctoPotato {
    class IApplication : public IRuntimeModule {
    public:
        virtual bool isQuit() = 0;
    };
}