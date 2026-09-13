#ifndef SCOP_HPP
# define SCOP_HPP
# include <iostream>
# include <vector>
# include <sstream>
# include <cstdint>
# include <cstring>
# include "scopexeption.hpp"
# include "m4.hpp"
# include <tuple>
# include <cmath>
# include <vulkan/vulkan.h>
# include <fstream>
# include <algorithm>
# define GLFW_INCLUDE_VULKAN
# include <array>
# include <GLFW/glfw3.h>
#include "camera.hpp"
#include "vector.hpp"
#include "material.hpp"
class material;
class Camera;
struct Vec3;
struct Vec2;
struct Vertex;
struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};
struct SubMesh {
    uint32_t indexOffset;
    uint32_t indexCount;
    int materialIndex;
};
void parse(std::string filename, std::vector<Vertex>& out_vertices, std::vector<uint32_t>& out_indices,
           std::vector<material>& vecmat, std::vector<SubMesh>& out_subMeshes);
           void parsemat(std::string filename, std::vector<material>& vecmat);
VkInstance createVulkanInstance();
GLFWwindow* initWindow();
VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow *window);
VkPhysicalDevice pickPhysicalDevice(VkInstance instance);
uint32_t findGraphicsQueueFamily(VkPhysicalDevice physicalDevice);
VkDevice createLogicalDevice(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkQueue* graphicsQueue);
//swapchain
struct SwapchainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};
SwapchainSupportDetails querySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& formats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& modes);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window);
VkSwapchainKHR createSwapchain(VkPhysicalDevice physicalDevice, VkDevice device, VkSurfaceKHR surface, GLFWwindow* window, VkFormat* outFormat, VkExtent2D* outExtent);
std::vector<VkImageView> createImageViews(VkDevice device, VkSwapchainKHR swapchain, VkFormat swapchainFormat);
VkRenderPass createRenderPass(VkDevice device, VkFormat swapchainFormat);
std::vector<VkFramebuffer> createFramebuffers(VkDevice device, VkRenderPass renderPass,
                                               const std::vector<VkImageView>& imageViews,
                                               VkExtent2D swapchainExtent);
VkDescriptorSetLayout createDescriptorSetLayout(VkDevice device);
void createGraphicsPipeline(VkDevice device, VkExtent2D swapchainExtent,
                             VkRenderPass renderPass, VkDescriptorSetLayout descriptorSetLayout,
                             VkPipelineLayout* outPipelineLayout, VkPipeline* outPipeline);
VkCommandPool createCommandPool(VkDevice device, uint32_t queueFamilyIndex);
VkCommandBuffer createCommandBuffer(VkDevice device, VkCommandPool commandPool);
struct SyncObjects {
    VkSemaphore imageAvailableSemaphore;
    VkSemaphore renderFinishedSemaphore;
    VkFence inFlightFence;
};
SyncObjects createSyncObjects(VkDevice device);
void drawFrame(VkDevice device, VkSwapchainKHR swapchain, VkQueue graphicsQueue,
               VkCommandBuffer commandBuffer, VkRenderPass renderPass,
               const std::vector<VkFramebuffer>& framebuffers, VkExtent2D swapchainExtent,
               VkPipeline graphicsPipeline, VkPipelineLayout pipelineLayout,
               VkBuffer vertexBuffer, VkBuffer indexBuffer,
               const std::vector<SubMesh>& subMeshes, const std::vector<material>& materials,
               VkDescriptorSet descriptorSet,
               void* uniformBufferMapped, const m4& mvp,
               SyncObjects& sync);
void createVertexBuffer(VkPhysicalDevice physicalDevice, VkDevice device,
                         VkCommandPool commandPool, VkQueue graphicsQueue,
                         const std::vector<Vertex>& vertices,
                         VkBuffer* outBuffer, VkDeviceMemory* outMemory);

void createIndexBuffer(VkPhysicalDevice physicalDevice, VkDevice device,
                        VkCommandPool commandPool, VkQueue graphicsQueue,
                        const std::vector<uint32_t>& indices,
                        VkBuffer* outBuffer, VkDeviceMemory* outMemory);
struct UniformBufferObject {
    m4 mvp;
};
void createUniformBuffer(VkPhysicalDevice physicalDevice, VkDevice device,
                          VkBuffer* outBuffer, VkDeviceMemory* outMemory, void** outMapped);
void updateUniformBuffer(void* mapped, const m4& mvp);
VkDescriptorPool createDescriptorPool(VkDevice device);
VkDescriptorSet createDescriptorSet(VkDevice device, 
                                    VkDescriptorPool descriptorPool, 
                                    VkDescriptorSetLayout descriptorSetLayout, 
                                    VkBuffer uniformBuffer,
                                    VkImageView textureImageView,
                                    VkSampler textureSampler);
//handle mouvement
void handleKeyboard(GLFWwindow *window, Camera& camera, float deltatime);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
VkFormat findDepthFormat(VkPhysicalDevice physicalDevice);

void createDepthResources(VkPhysicalDevice physicalDevice, VkDevice device,
                           VkExtent2D swapchainExtent, VkFormat depthFormat,
                           VkImage* outImage, VkDeviceMemory* outMemory, VkImageView* outView);

VkRenderPass createRenderPass(VkDevice device, VkFormat swapchainFormat, VkFormat depthFormat);

std::vector<VkFramebuffer> createFramebuffers(VkDevice device, VkRenderPass renderPass,
                                               const std::vector<VkImageView>& imageViews,
                                               VkImageView depthImageView,
                                               VkExtent2D swapchainExtent);
void createTextureImage(VkPhysicalDevice physicalDevice, VkDevice device,
                         VkCommandPool commandPool, VkQueue graphicsQueue,
                         const std::string& bmpPath,
                         VkImage* outImage, VkDeviceMemory* outMemory);
void transitionImageLayout(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue,
                            VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout);
void copyBufferToImage(VkDevice device, VkCommandPool commandPool, VkQueue graphicsQueue,
                        VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
VkImageView createTextureImageView(VkDevice device, VkImage textureImage);
VkSampler createTextureSampler(VkDevice device);
struct ImageData {
    int width;
    int height;
    int channels;
    std::vector<unsigned char> pixels;
};

ImageData parseBMP(const std::string& path);

void createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, 
                  VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, 
                  VkBuffer& buffer, VkDeviceMemory& bufferMemory);

uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, 
                        VkMemoryPropertyFlags properties);
#endif