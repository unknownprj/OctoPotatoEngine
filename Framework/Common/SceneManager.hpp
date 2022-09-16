#pragma once
#include "IRuntimeModule.hpp"
// #include "SceneParser.hpp"

namespace OctoPotato {
    class SceneManager : implements IRuntimeModule
    {
    public:
        virtual ~SceneManager();

        virtual int initialize();
        virtual void finalize();

        virtual void tick();

        void loadScene(const char *sceneFile);

        // const Scene& GetSceneForRendering();

    protected:
        // void loadOgexScene(const char* scene_file_name);

        // std::unique_ptr<Scene> m_pScene;
    };
}

