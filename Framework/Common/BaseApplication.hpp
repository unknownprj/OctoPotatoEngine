#pragma once
#include "IApplication.hpp"
#include "Configuration.hpp"
#include <iostream>

namespace OctoPotato {
    class BaseApplication : implements IApplication {
    public:
        BaseApplication(const Configuration& config);
        virtual int initialize();
        virtual void finalize();
        virtual void tick();
        virtual bool isQuit();
        static bool m_bQuit;
        Configuration m_config;
    private:
        BaseApplication();
    };
}