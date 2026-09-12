#include "includes/scop.hpp"
#include "includes/camera.hpp"

void handleKeyboard(GLFWwindow *window, Camera& camera, float deltatime)
{
    float speed = 2.5f * deltatime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.move(camera.getfront() * speed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.move(camera.getfront() * -speed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.move(camera.getright() * -speed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.move(camera.getright() * speed);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    static float lastX = 400, lastY = 300;
    static bool firstMouse = true;

    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));

    if (firstMouse) {
        lastX = (float)xpos;
        lastY = (float)ypos;
        firstMouse = false;
    }

    float xoffset = (float)xpos - lastX;
    float yoffset = lastY - (float)ypos;
    lastX = (float)xpos;
    lastY = (float)ypos;

    float sensitivity = 0.002f;
    camera->rotate(xoffset * sensitivity, -(yoffset * sensitivity));
}