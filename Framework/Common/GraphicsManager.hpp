#pragma once
#include "IRuntimeModule.hpp"
#include <iostream>

namespace OctoPotato {
    class GraphicsManager : public IRuntimeModule {
    public:
        GraphicsManager() { std::clog << "GraphicsManaCons" << std::endl; }
        virtual ~GraphicsManager() {}

        virtual int initialize();
        virtual void finalize();

        virtual void tick();

        virtual void prepareSceneRelatedGraphicsResources(){};

        virtual void releaseSceneRelatedGraphicsResources(){};

        virtual void clear();
        virtual void draw();
    };
}