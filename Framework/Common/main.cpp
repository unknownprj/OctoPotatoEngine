#include <stdio.h>
#include "IApplication.hpp"
#include "GraphicsManager.hpp"
#include "MemoryManager.hpp"

using namespace OctoPotato;

namespace OctoPotato {
    extern IApplication* g_pApp;
    extern MemoryManager *g_pMemManager;
    extern GraphicsManager *g_pGraphicsManager;
}

int main(int argc, char** argv) {
    int ret;

    if((ret = g_pApp->initialize()) != 0) {
        printf("App Initialization failed, will exit...");
        return ret;
    }

    if((ret = g_pMemManager->initialize()) != 0) {
        printf("Memory manager Initialization failed, will exit...");
        return ret;
    }

    if((ret = g_pGraphicsManager->initialize()) != 0) {
        printf("Graphics manager Initialization failed, will exit...");
        return ret;
    }

    while(!g_pApp->isQuit()) {
        g_pApp->tick();
        g_pMemManager->tick();
        g_pGraphicsManager->tick();
    }

    g_pGraphicsManager->finalize();
    g_pMemManager->finalize();
    g_pApp->finalize();

    return 0;
}