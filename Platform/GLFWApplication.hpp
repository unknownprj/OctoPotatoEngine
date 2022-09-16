#pragma once
#define GLFW_INCLUDE_VULKAN
#include "BaseApplication.hpp"
#include <GLFW/glfw3.h>

namespace OctoPotato {
    class GLFWApplication : public BaseApplication {
    public:
        GLFWApplication(const Configuration &cfg) : BaseApplication(cfg), window(nullptr) { std::clog << "GLFWAppCons" << std::endl; };

        virtual int initialize();
        virtual void finalize();
        virtual void tick();
        virtual bool isQuit();

        GLFWwindow *getGLFWwindowHandle() const { return window; };

        static void glfwErrorCallback(int error, const char* description);
        static void glfwWindowCloseCallback(GLFWwindow* window);
        static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void glfwFrameBufferSizeCallback(GLFWwindow *window, int width, int height);
    private:
        GLFWwindow* window;
    };
}