#include "SceneManager.hpp"
#include "AssetLoader.hpp"
// #include "OGEX.hpp"
#include "global.hpp"
#include "VulkanGraphicsManager.hpp"
#include <iostream>

using namespace OctoPotato;
using namespace std;

SceneManager::~SceneManager()
{
}

int SceneManager::initialize()
{
    int result = 0;
    return result;
}

void SceneManager::finalize()
{
}

void SceneManager::tick()
{
}

void SceneManager::unloadScene() {
	delete p_glTFScene;
}

void SceneManager::loadScene(const char *sceneFile) {
	std::cerr << "loading scene file: " << sceneFile << std::endl;
    // loadOgexScene(sceneFile);
    loadglTFFile(sceneFile);
}

void SceneManager::loadglTFFile(std::string filename) {
    tinygltf::Model glTFInput;
	tinygltf::TinyGLTF gltfContext;
	std::string error, warning;
	p_glTFScene = new VulkanglTFScene;
#if defined(__ANDROID__)
	// On Android all assets are packed with the apk in a compressed form, so we need to open them using the asset manager
	// We let tinygltf handle this, by passing the asset manager of our app
	tinygltf::asset_manager = androidApp->activity->assetManager;
#endif
    bool fileLoaded = gltfContext.LoadASCIIFromFile(&glTFInput, &error, &warning, filename);
	vks::VulkanDevice *vulkanDevice = reinterpret_cast<VulkanGraphicsManager *>(g_pGraphicsManager)->vulkanDevice;
	VkQueue queue = reinterpret_cast<VulkanGraphicsManager *>(g_pGraphicsManager)->queue;
	// Pass some Vulkan resources required for setup and rendering to the glTF model loading class
	p_glTFScene->vulkanDevice = vulkanDevice;
	p_glTFScene->copyQueue    = queue;
	VkDevice device = vulkanDevice->logicalDevice;
	size_t pos = filename.find_last_of('/');
	p_glTFScene->path = filename.substr(0, pos);

	std::vector<uint32_t> indexBuffer;
	std::vector<VulkanglTFScene::Vertex> vertexBuffer;
	VulkanglTFScene &glTFScene = *p_glTFScene;
	if (fileLoaded) {
		glTFScene.loadImages(glTFInput);
		glTFScene.loadMaterials(glTFInput);
		glTFScene.loadTextures(glTFInput);
		const tinygltf::Scene& scene = glTFInput.scenes[0];
		for (size_t i = 0; i < scene.nodes.size(); i++) {
			const tinygltf::Node node = glTFInput.nodes[scene.nodes[i]];
			glTFScene.loadNode(node, glTFInput, nullptr, indexBuffer, vertexBuffer);
		}
#if defined(DEBUG)
		std::clog << indexBuffer.size() << " indices loaded, "
				<< vertexBuffer.size() << " vertices loaded" << std::endl;
#endif
	}
	else {
		std::cerr << error << std::endl;
		vks::tools::exitFatal("Could not open the glTF file.\n\nThe file is part of the additional asset pack.\n\nRun \"download_assets.py\" in the repository root to download the latest version.", -1);
		return;
	}

	// Create and upload vertex and index buffer
	// We will be using one single vertex buffer and one single index buffer for the whole glTF scene
	// Primitives (of the glTF model) will then index into these using index offsets

	size_t vertexBufferSize = vertexBuffer.size() * sizeof(VulkanglTFScene::Vertex);
	size_t indexBufferSize = indexBuffer.size() * sizeof(uint32_t);
	glTFScene.indices.count = static_cast<uint32_t>(indexBuffer.size());

	struct StagingBuffer {
		VkBuffer buffer;
		VkDeviceMemory memory;
	} vertexStaging, indexStaging;

	// Create host visible staging buffers (source)
	VK_CHECK_RESULT(vulkanDevice->createBuffer(
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		vertexBufferSize,
		&vertexStaging.buffer,
		&vertexStaging.memory,
		vertexBuffer.data()));
	// Index data
	VK_CHECK_RESULT(vulkanDevice->createBuffer(
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		indexBufferSize,
		&indexStaging.buffer,
		&indexStaging.memory,
		indexBuffer.data()));

	// Create device local buffers (target)
	VK_CHECK_RESULT(vulkanDevice->createBuffer(
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		vertexBufferSize,
		&glTFScene.vertices.buffer,
		&glTFScene.vertices.memory));
	VK_CHECK_RESULT(vulkanDevice->createBuffer(
		VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		indexBufferSize,
		&glTFScene.indices.buffer,
		&glTFScene.indices.memory));

	// Copy data from staging buffers (host) do device local buffer (gpu)
	VkCommandBuffer copyCmd = vulkanDevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
	VkBufferCopy copyRegion = {};

	copyRegion.size = vertexBufferSize;
	vkCmdCopyBuffer(
		copyCmd,
		vertexStaging.buffer,
		glTFScene.vertices.buffer,
		1,
		&copyRegion);

	copyRegion.size = indexBufferSize;
	vkCmdCopyBuffer(
		copyCmd,
		indexStaging.buffer,
		glTFScene.indices.buffer,
		1,
		&copyRegion);

	vulkanDevice->flushCommandBuffer(copyCmd, queue, true);

	// Free staging resources
	vkDestroyBuffer(device, vertexStaging.buffer, nullptr);
	vkFreeMemory(device, vertexStaging.memory, nullptr);
	vkDestroyBuffer(device, indexStaging.buffer, nullptr);
	vkFreeMemory(device, indexStaging.memory, nullptr);
}

// void SceneManager::loadOgexScene(const char* scene_file_name)
// {
//     string ogex_text = g_pAssetLoader->syncOpenAndReadTextFileToString(scene_file_name);

//     OgexParser ogex_parser;
//     m_pScene = ogex_parser.parse(ogex_text);
// }

// const Scene& SceneManager::getSceneForRendering() {
//     return *m_pScene;
// }