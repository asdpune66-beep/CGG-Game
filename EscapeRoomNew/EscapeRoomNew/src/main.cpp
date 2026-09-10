#include <iostream>
#include <chrono>

#include <GLFW/glfw3.h>

#include "core/Game.h"


Game* game = nullptr;


// =============================================================
// WINDOW RESIZE CALLBACK
// =============================================================

void framebufferSizeCallback(
    GLFWwindow* window,
    int width,
    int height
)
{
    if (!game)
        return;


    game->resize(
        width,
        height
    );
}


// =============================================================
// KEYBOARD CALLBACK
// =============================================================

void keyCallback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods
)
{
    // ---------------------------------------------------------
    // Escape closes the game
    // ---------------------------------------------------------

    if (key == GLFW_KEY_ESCAPE &&
        action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(
            window,
            GLFW_TRUE
        );

        return;
    }


    // ---------------------------------------------------------
    // Make sure the Game object exists
    // ---------------------------------------------------------

    if (!game)
        return;


    // ---------------------------------------------------------
    // Key pressed
    // ---------------------------------------------------------

    if (action == GLFW_PRESS)
    {
        game->keyPressed(key);
    }


    // ---------------------------------------------------------
    // Key released
    // ---------------------------------------------------------

    else if (action == GLFW_RELEASE)
    {
        game->keyReleased(key);
    }
}


// =============================================================
// MAIN
// =============================================================

int main()
{
    // ---------------------------------------------------------
    // GLFW INITIALIZATION
    // ---------------------------------------------------------

    if (!glfwInit())
    {
        std::cerr
            << "Failed to initialize GLFW.\n";

        return -1;
    }


    // ---------------------------------------------------------
    // OPENGL CONFIGURATION
    // ---------------------------------------------------------

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MAJOR,
        2
    );

    glfwWindowHint(
        GLFW_CONTEXT_VERSION_MINOR,
        1
    );

    glfwWindowHint(
        GLFW_RESIZABLE,
        GLFW_TRUE
    );


    // ---------------------------------------------------------
    // CREATE WINDOW
    // ---------------------------------------------------------

    GLFWwindow* window = glfwCreateWindow(
        1280,
        720,
        "Escape Room - Protocol Blackout",
        nullptr,
        nullptr
    );


    if (!window)
    {
        std::cerr
            << "Failed to create OpenGL window.\n";

        glfwTerminate();

        return -1;
    }


    // ---------------------------------------------------------
    // OPENGL CONTEXT
    // ---------------------------------------------------------

    glfwMakeContextCurrent(window);


    // ---------------------------------------------------------
    // ENABLE V-SYNC
    // ---------------------------------------------------------

    glfwSwapInterval(1);


    // ---------------------------------------------------------
    // CREATE GAME
    // ---------------------------------------------------------

    Game gameInstance(window);

    game = &gameInstance;


    // ---------------------------------------------------------
    // CALLBACKS
    // ---------------------------------------------------------

    glfwSetFramebufferSizeCallback(
        window,
        framebufferSizeCallback
    );

    glfwSetKeyCallback(
        window,
        keyCallback
    );


    // =========================================================
    // GAME LOOP
    // =========================================================

    auto previousTime =
        std::chrono::high_resolution_clock::now();


    while (!glfwWindowShouldClose(window))
    {
        // -----------------------------------------------------
        // CALCULATE DELTA TIME
        // -----------------------------------------------------

        auto currentTime =
            std::chrono::high_resolution_clock::now();


        std::chrono::duration<double> elapsed =
            currentTime - previousTime;


        previousTime = currentTime;


        const double deltaTime =
            elapsed.count();


        // -----------------------------------------------------
        // PROCESS INPUT / EVENTS
        // -----------------------------------------------------

        glfwPollEvents();


        // -----------------------------------------------------
        // UPDATE GAME
        // -----------------------------------------------------

        game->update(deltaTime);


        // -----------------------------------------------------
        // RENDER GAME
        // -----------------------------------------------------

        game->render();
    }


    // =========================================================
    // CLEANUP
    // =========================================================

    glfwDestroyWindow(window);

    glfwTerminate();


    return 0;
}