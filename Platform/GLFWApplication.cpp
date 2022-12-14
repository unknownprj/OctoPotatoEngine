#include "GLFWApplication.hpp"
#include <stdexcept>
#include <iostream>
#include "global.hpp"
#include "VulkanGraphicsManager.hpp"

using namespace OctoPotato;

int GLFWApplication::initialize() {
    if(!glfwInit()) {
        throw std::runtime_error("GLFW initialization failed");
    }
    glfwSetErrorCallback(GLFWApplication::glfwErrorCallback);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    window = glfwCreateWindow(m_config.k_width, m_config.k_height, m_config.title, nullptr, nullptr);
    if(!window) {
        glfwTerminate();
        throw std::runtime_error("creating window failed");
    }
    glfwSetWindowUserPointer(window, g_pGraphicsManager);
    glfwSetFramebufferSizeCallback(window, VulkanGraphicsManager::framebufferResizeCallback);
    glfwSetCursorPosCallback(window, VulkanGraphicsManager::cursorPositionCallback);
    glfwSetMouseButtonCallback(window, VulkanGraphicsManager::mouseButtonCallback);
    glfwSetKeyCallback(window, VulkanGraphicsManager::keyCallback);
    glfwSetScrollCallback(window, VulkanGraphicsManager::scrollCallback);

    return 0;
}

void GLFWApplication::finalize() {
    if(window) {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

void GLFWApplication::glfwErrorCallback(int error, const char *msg) {
    std::cerr << "[GLFW | ERROR:" << error << "] " << msg << std::endl;
}

bool GLFWApplication::isQuit() { return glfwWindowShouldClose(window); }

void GLFWApplication::tick() {
    glfwPollEvents();
}