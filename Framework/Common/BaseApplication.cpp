#include "BaseApplication.hpp"

int OctoPotato::BaseApplication::initialize() {
    m_bQuit = false;

    return 0;
}

void OctoPotato::BaseApplication::finalize() {}

void OctoPotato::BaseApplication::tick() {}

bool OctoPotato::BaseApplication::isQuit() { return m_bQuit; }