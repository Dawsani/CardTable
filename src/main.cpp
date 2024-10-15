#include "Engine.h"

Engine* pEngine;

int main() {
    pEngine = new Engine();
    pEngine->run();
    return 0;
}

void cursor_position_callback(GLFWwindow* window, double x, double y ) {
    pEngine->handleCursorPositionEvent(x, y);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    pEngine->handleFramebufferSizeEven(width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods ) {
    pEngine->handleMouseButtonEvent(button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xOffset, double yOffset) {
    pEngine->handleScrollEvent(xOffset, yOffset);
}
