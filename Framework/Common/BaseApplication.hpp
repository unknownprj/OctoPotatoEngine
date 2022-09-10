#pragma once
#include "IApplication.hpp"
#include "Configuration.hpp"

namespace OctoPotato {
    class BaseApplication : implements IApplication {
    public:
        BaseApplication(const Configuration& config);
        virtual int initialize();
        virtual void finalize();
        virtual void tick();
        virtual bool isQuit();
    protected:
        static bool m_bQuit;
        Configuration m_config;
    private:
        BaseApplication();
    };
}