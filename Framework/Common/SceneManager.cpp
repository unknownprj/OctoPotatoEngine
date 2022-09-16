#include "SceneManager.hpp"
#include "AssetLoader.hpp"
// #include "OGEX.hpp"
#include "global.hpp"

using namespace OctoPotato;
using namespace std;

SceneManager::~SceneManager()
{
}

int SceneManager::initialize()
{
    int result = 0;
    return result;
}

void SceneManager::finalize()
{
}

void SceneManager::tick()
{
}

void SceneManager::loadScene(const char *sceneFile) {
    // loadOgexScene(sceneFile);
}

// void SceneManager::loadOgexScene(const char* scene_file_name)
// {
//     string ogex_text = g_pAssetLoader->syncOpenAndReadTextFileToString(scene_file_name);

//     OgexParser ogex_parser;
//     m_pScene = ogex_parser.parse(ogex_text);
// }

// const Scene& SceneManager::getSceneForRendering() {
//     return *m_pScene;
// }