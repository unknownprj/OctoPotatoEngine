#include "BaseApplication.hpp"

namespace OctoPotato {
    BaseApplication g_App;
    IApplication* g_pApp = &g_App;
}