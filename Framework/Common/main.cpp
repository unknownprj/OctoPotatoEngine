#include <cstdio>
#include <chrono>
#include <thread>
#include "global.hpp"

using namespace OctoPotato;

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

    if((ret = g_pAssetLoader->initialize()) != 0) {
        printf("Asset loader initialization failed, will exit...");
        return ret;
    }

    if((ret = g_pSceneManager->initialize()) != 0) {
        printf("Scene manager initialization failed, will exit...");
        return ret;
    }

    g_pSceneManager->loadScene("");

    if((ret = g_pGraphicsManager->initialize()) != 0) {
        printf("Graphics manager Initialization failed, will exit...");
        return ret;
    }

    while(!g_pApp->isQuit()) {
        g_pApp->tick();
        g_pMemManager->tick();
        g_pAssetLoader->tick();
        // g_pSceneManager->tick();
        g_pGraphicsManager->tick();
        std::this_thread::sleep_for(std::chrono::microseconds(10000));
    }

    g_pGraphicsManager->finalize();
    // g_pSceneManager->finalize();
    g_pAssetLoader->finalize();
    g_pMemManager->finalize();
    g_pApp->finalize();

    return 0;
}