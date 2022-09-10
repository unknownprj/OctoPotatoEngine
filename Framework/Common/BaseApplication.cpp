#include "BaseApplication.hpp"

bool OctoPotato::BaseApplication::m_bQuit = false;

OctoPotato::BaseApplication::BaseApplication(const Configuration& cfg) : m_config(cfg) {}

int OctoPotato::BaseApplication::initialize() {
    return 0;
}

void OctoPotato::BaseApplication::finalize() {}

void OctoPotato::BaseApplication::tick() {}

bool OctoPotato::BaseApplication::isQuit() { return m_bQuit; }