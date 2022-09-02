#include <stdio.h>
#include "IApplication.hpp"

using namespace OctoPotato;

namespace OctoPotato {
    extern IApplication* g_pApp;
}

int main(int argc, char** argv) {
    int ret;

    if((ret = g_pApp->initialize()) != 0) {
        printf("App Initialization failed, will exit...");
        return ret;
    }

    while(!g_pApp->isQuit()) {
        g_pApp->tick();
    }

    g_pApp->finalize();

    return 0;
}