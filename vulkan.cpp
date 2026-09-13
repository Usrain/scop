#include "includes/scop.hpp"

VkInstance createVulkanInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "scop";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // GLFW te dit quelles extensions il a besoin (pour créer une surface plus tard)
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0; // pas de validation layers pour l'instant

    VkInstance instance;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS)
        throw std::runtime_error("failed to create Vulkan instance");
    return (instance);
}

GLFWwindow* initWindow()
{
    GLFWwindow* window;

    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    window = glfwCreateWindow(800, 600, "scop", nullptr, nullptr);

    return (window);
}

VkSurfaceKHR createSurface(VkInstance instance, GLFWwindow *window)
{
    VkSurfaceKHR surface;

    if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS)
        throw std::runtime_error("failed to create window surface");
    return (surface);
}

bool isDeviceSuitable(VkPhysicalDevice device)
{
    // vérifie qu'il a bien une queue family graphics
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    bool hasGraphics = false;
    for (const auto& qf : queueFamilies) {
        if (qf.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            hasGraphics = true;
    }
    if (!hasGraphics)
        return false;
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    bool hasSwapchain = false;
    for (const auto& ext : availableExtensions) {
        if (strcmp(ext.extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0)
            hasSwapchain = true;
    }
    return hasSwapchain;
}

int rateDevice(VkPhysicalDevice device)
{
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(device, &props);
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(device, &features);
    int score = 0;
    if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        score += 1000;
    else if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU)
        score += 100;
    score += props.limits.maxImageDimension2D;

    return score;
}

VkPhysicalDevice pickPhysicalDevice(VkInstance instance)
{
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0)
        throw std::runtime_error("no GPU with Vulkan support found");
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());
    VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
    int bestScore = -1;
    for (const auto& device : devices) {
        if (!isDeviceSuitable(device))
            continue;
        int score = rateDevice(device);
        if (score > bestScore) {
            bestScore = score;
            bestDevice = device;
        }
    }
    if (bestDevice == VK_NULL_HANDLE)
        throw std::runtime_error("no suitable GPU found");

    return bestDevice;
}

uint32_t findGraphicsQueueFamily(VkPhysicalDevice physicalDevice)
{
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    for (uint32_t i = 0; i < queueFamilyCount; i++) {
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
            return i;
    }
    throw std::runtime_error("no graphics queue family found");
}

VkDevice createLogicalDevice(VkPhysicalDevice physicalDevice, uint32_t queueFamilyIndex, VkQueue* graphicsQueue)
{
    float queuePriority = 1.0f;

    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures{}; // rien de spécial pour l'instant

    const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledExtensionCount = (uint32_t)deviceExtensions.size();
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    VkDevice device;
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS)
        throw std::runtime_error("failed to create logical device");

    vkGetDeviceQueue(device, queueFamilyIndex, 0, graphicsQueue);
    return device;
}
#include "includes/scop.hpp"
#include <stdexcept>



// Crée un VkBuffer et lui alloue la mémoire GPU correspondante
void createBuffer(VkPhysicalDevice physicalDevice, VkDevice device, VkDeviceSize size, 
                  VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, 
                  VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    // 1. Création du handle de Buffer
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE; // Utilisé par une seule queue family

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("Échec de la création du buffer !");
    }

    // 2. Récupération des contraintes mémoire requises par le buffer
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    // 3. Configuration de l'allocation mémoire
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

    // 4. Allocation de la mémoire sur la carte graphique
    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("Échec de l'allocation de la mémoire du buffer !");
    }

    // 5. Liaison (bind) entre le buffer et la mémoire allouée
    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

ImageData parseBMP(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier BMP : " + filename);
    }

    unsigned char header[54];
    file.read(reinterpret_cast<char*>(header), 54);
    if (file.gcount() < 54 || header[0] != 'B' || header[1] != 'M') {
        throw std::runtime_error("Fichier BMP invalide ou corrompu : " + filename);
    }

    uint32_t dataOffset = *reinterpret_cast<uint32_t*>(&header[10]);
    int width           = *reinterpret_cast<int*>(&header[18]);
    int height          = *reinterpret_cast<int*>(&header[22]);
    uint16_t bpp        = *reinterpret_cast<uint16_t*>(&header[28]);

    if (bpp != 24 && bpp != 32) {
        throw std::runtime_error("Seuls les formats BMP 24-bit et 32-bit sont supportés.");
    }

    int channels = bpp / 8;
    uint32_t imageSize = width * height * 4; // Format RGBA forcé pour Vulkan

    file.seekg(dataOffset, std::ios::beg);
    std::vector<unsigned char> rawData(width * height * channels);
    file.read(reinterpret_cast<char*>(rawData.data()), rawData.size());

    std::vector<unsigned char> pixels(imageSize);

    // Convertit BGR(A) vers RGBA et inverse les lignes (BMP stocke de bas en haut)
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int srcIndex = ((height - 1 - y) * width + x) * channels;
            int dstIndex = (y * width + x) * 4;

            pixels[dstIndex + 0] = rawData[srcIndex + 2]; // Red
            pixels[dstIndex + 1] = rawData[srcIndex + 1]; // Green
            pixels[dstIndex + 2] = rawData[srcIndex + 0]; // Blue
            pixels[dstIndex + 3] = (channels == 4) ? rawData[srcIndex + 3] : 255; // Alpha
        }
    }

    return ImageData{width, height, 4, pixels};
}