#pragma once
#include "IApplication.hpp"
#include "GraphicsManager.hpp"
#include "MemoryManager.hpp"
#include "AssetLoader.hpp"
#include "SceneManager.hpp"

namespace OctoPotato {
    extern IApplication *g_pApp;
    extern GraphicsManager *g_pGraphicsManager;
    extern MemoryManager *g_pMemManager;
    extern AssetLoader *g_pAssetLoader;
    extern SceneManager *g_pSceneManager;
}