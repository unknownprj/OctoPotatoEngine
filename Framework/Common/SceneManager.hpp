#pragma once
#include "IRuntimeModule.hpp"
#include "gltfSceneParser.hpp"
// #include "SceneParser.hpp"

namespace OctoPotato {
    class SceneManager : public IRuntimeModule
    {
    public:
        virtual ~SceneManager();

        virtual int initialize();
        virtual void finalize();

        virtual void tick();

        void loadScene(const char *sceneFile);

        void unloadScene();

        // const Scene& GetSceneForRendering();
        VulkanglTFScene* getgltfScene() { return p_glTFScene; }

    protected:
        VulkanglTFScene* p_glTFScene;

        void loadglTFFile(std::string filename);
        // void loadOgexScene(const char* scene_file_name);

        // std::unique_ptr<Scene> m_pScene;
    };
}

