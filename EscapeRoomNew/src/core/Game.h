#pragma once

#include <GLFW/glfw3.h>

class Renderer;

class Game
{
public:
    explicit Game(GLFWwindow* window);

    void update(double deltaTime);
    void render();

    void keyPressed(int key);
    void keyReleased(int key);

    void resize(int width, int height);

private:
    GLFWwindow* window;
    Renderer* renderer;

    bool keyState[GLFW_KEY_LAST + 1]{};

    double playerX = 640.0;
    double playerY = 360.0;
};