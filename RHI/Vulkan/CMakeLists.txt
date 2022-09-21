add_subdirectory(base)

add_library(VkManager
VulkanGraphicsManager.cpp)

target_link_libraries(VkManager base)