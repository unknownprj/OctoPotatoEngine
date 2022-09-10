#pragma once
#include <cstdint>

namespace OctoPotato {
    struct Configuration {
        const char *title;

        uint32_t k_width;
        uint32_t k_height;

        bool b_fullscreen;
        bool b_validation;
        bool b_vsync;

        uint32_t k_msaaSamples;

        Configuration(uint32_t width, uint32_t height, const char *windowTitle = "Application", bool fullscreen = false, bool validation = true, bool vsync = false, uint32_t msaa = 0) :
            title(windowTitle), k_width(width), k_height(height), b_fullscreen(fullscreen), b_validation(validation), b_vsync(vsync), k_msaaSamples(msaa) {}
        
    };
}