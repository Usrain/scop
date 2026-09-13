#include "includes/scop.hpp"
#include "includes/camera.hpp"
#include <cstdint>

int main(int ac, char **av)
{
    std::vector<Vertex> finalVertices;
    std::vector<uint32_t> finalIndices;
    std::vector<material> vecmat;
    std::vector<SubMesh> finalSubMeshes;
    if (ac != 2)
        return 1;
    try {
        parse(av[1], finalVertices, finalIndices, vecmat, finalSubMeshes);
    }
    catch (ParsingException e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    std::cout << "=== PARSING TERMINÉ ===" << std::endl;
    std::cout << "Nombre total de vertices : " << finalVertices.size() << std::endl;
    std::cout << "Nombre total d'indices  : " << finalIndices.size() << std::endl;
    std::cout << "Nombre total de materiau  :" << vecmat.size() << std::endl;
    std::cout << "=======================\n" << std::endl;
    std::cout << "Nombre de SubMeshes : " << finalSubMeshes.size() << std::endl;
    for (const auto& sub : finalSubMeshes)
        std::cout << "  materialIndex=" << sub.materialIndex << " count=" << sub.indexCount << std::endl;
    for (size_t i = 0; i < vecmat.size(); i++)
        std::cout << "Material " << i << " Kd = " << vecmat[i].Kd.x << " " << vecmat[i].Kd.y << " " << vecmat[i].Kd.z << std::endl;

    GLFWwindow *window = initWindow();
    VkInstance instance = createVulkanInstance();
    VkSurfaceKHR surface = createSurface(instance, window);
    VkPhysicalDevice physicalDevice = pickPhysicalDevice(instance);
    uint32_t queueFamilyIndex = findGraphicsQueueFamily(physicalDevice);
    VkQueue graphicsQueue;
    VkDevice device = createLogicalDevice(physicalDevice, queueFamilyIndex, &graphicsQueue);

    VkFormat swapchainFormat;
    VkExtent2D swapchainExtent;
    VkSwapchainKHR swapchain = createSwapchain(physicalDevice, device, surface, window, &swapchainFormat, &swapchainExtent);
    std::vector<VkImageView> imageViews = createImageViews(device, swapchain, swapchainFormat);
    
    VkFormat depthFormat = findDepthFormat(physicalDevice);
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    createDepthResources(physicalDevice, device, swapchainExtent, depthFormat,
                          &depthImage, &depthImageMemory, &depthImageView);

    VkRenderPass renderPass = createRenderPass(device, swapchainFormat, depthFormat);
    std::vector<VkFramebuffer> framebuffers = createFramebuffers(device, renderPass, imageViews, depthImageView, swapchainExtent);

    VkDescriptorSetLayout descriptorSetLayout = createDescriptorSetLayout(device);
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
    createGraphicsPipeline(device, swapchainExtent, renderPass, descriptorSetLayout,
                        &pipelineLayout, &graphicsPipeline);

    VkCommandPool commandPool = createCommandPool(device, queueFamilyIndex);
    VkCommandBuffer commandBuffer = createCommandBuffer(device, commandPool);
    SyncObjects sync = createSyncObjects(device);

    // Buffers (Vertex, Index, Uniform)
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    createVertexBuffer(physicalDevice, device, commandPool, graphicsQueue,
                        finalVertices, &vertexBuffer, &vertexBufferMemory);

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;
    createIndexBuffer(physicalDevice, device, commandPool, graphicsQueue,
                    finalIndices, &indexBuffer, &indexBufferMemory);

    VkBuffer uniformBuffer;
    VkDeviceMemory uniformBufferMemory;
    void* uniformBufferMapped;
    createUniformBuffer(physicalDevice, device, &uniformBuffer, &uniformBufferMemory, &uniformBufferMapped);

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    createTextureImage(physicalDevice, device, commandPool, graphicsQueue, "texture.bmp", &textureImage, &textureImageMemory);
    VkImageView textureImageView = createTextureImageView(device, textureImage);
    VkSampler textureSampler = createTextureSampler(device);

    VkDescriptorPool descriptorPool = createDescriptorPool(device);
    VkDescriptorSet descriptorSet = createDescriptorSet(device, descriptorPool, descriptorSetLayout, uniformBuffer, textureImageView, textureSampler);

    Camera camera(
        Vec3{0.0f, 0.0f, 3.0f},
        Vec3{0.0f, 0.0f, 0.0f},
        Vec3{0.0f, 1.0f, 0.0f},
        45.0f * (M_PI / 180.0f),
        (float)swapchainExtent.width / (float)swapchainExtent.height,
        0.1f,
        1000.0f
    );

    float lastFrame = 0.0f;
    glfwSetWindowUserPointer(window, &camera);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = (float)glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        handleKeyboard(window, camera, deltaTime);

        m4 model = m4::identity();
        m4 view = camera.getView();
        m4 proj = camera.getProjection();
        m4 mvp = proj * view * model;

        drawFrame(device, swapchain, graphicsQueue, commandBuffer, renderPass,
          framebuffers, swapchainExtent, graphicsPipeline, pipelineLayout,
          vertexBuffer, indexBuffer,
          finalSubMeshes, vecmat,
          descriptorSet, uniformBufferMapped, mvp, sync);
    }

    vkDeviceWaitIdle(device);

    vkDestroySampler(device, textureSampler, nullptr);
    vkDestroyImageView(device, textureImageView, nullptr);
    vkDestroyImage(device, textureImage, nullptr);
    vkFreeMemory(device, textureImageMemory, nullptr);

    vkDestroyImageView(device, depthImageView, nullptr);
    vkDestroyImage(device, depthImage, nullptr);
    vkFreeMemory(device, depthImageMemory, nullptr);

    return 0;
}