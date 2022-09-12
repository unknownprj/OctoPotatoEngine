#pragma once
#include <cstdint>

namespace OctoPotato {
    struct Image2D {
        uint32_t width;
        uint32_t height;
        uint8_t *image;
        uint32_t pixelBytes;
        uint32_t pitch;
        uint32_t pixelChannels;
        size_t imageSize;
    };
}