#include "GLFWApplication.hpp"
#include "VulkanGraphicsManager.hpp"
#include "global.hpp"

namespace OctoPotato {
    GLFWApplication g_App(Configuration(800U, 600U));
    IApplication* g_pApp = &g_App;
    MemoryManager *g_pMemManager = new MemoryManager();
    AssetLoader *g_pAssetLoader = new AssetLoader();
    SceneManager *g_pSceneManager = new SceneManager();
    GraphicsManager *g_pGraphicsManager = new VulkanGraphicsManager();
}