#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

int main() {
    // 初始化GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // 创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "GLFW Test Window", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // 设置窗口上下文
    glfwMakeContextCurrent(window);

    // 主循环
    while (!glfwWindowShouldClose(window)) {
        // 渲染
        glClear(GL_COLOR_BUFFER_BIT);

        // 交换缓冲区
        glfwSwapBuffers(window);

        // 处理事件
        glfwPollEvents();
    }

    // 清理
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
