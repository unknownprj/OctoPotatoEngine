add_executable(Test TestApplication.cpp)
find_library(VULKAN vulkan-1 "D:/VulkanSDK/1.2.182.0/Lib")
find_library(GLFW glfw3 "C:/lib/glfw-3.3.4.bin.WIN64/lib-mingw-w64")
target_link_libraries(Test Common Application ${VULKAM} ${GLFW} gdi32)