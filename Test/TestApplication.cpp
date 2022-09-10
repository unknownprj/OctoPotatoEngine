#include "GLFWApplication.hpp"

namespace OctoPotato {
    GLFWApplication g_App(Configuration(800U, 600U));
    IApplication* g_pApp = &g_App;
}