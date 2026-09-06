#include "Game.h"

#include "../graphics/Renderer.h"


// ============================================================
// CONSTRUCTOR
// ============================================================

Game::Game(GLFWwindow* window)
    : window(window)
{
    renderer =
        new Renderer(window);
}


// ============================================================
// KEY PRESSED
// ============================================================

void Game::keyPressed(int key)
{
    if (key >= 0 &&
        key <= GLFW_KEY_LAST)
    {
        keyState[key] = true;
    }
}


// ============================================================
// KEY RELEASED
// ============================================================

void Game::keyReleased(int key)
{
    if (key >= 0 &&
        key <= GLFW_KEY_LAST)
    {
        keyState[key] = false;
    }
}

// ============================================================
// WINDOW RESIZE
// ============================================================

void Game::resize(
    int width,
    int height
)
{
    renderer->resize(
        width,
        height
    );
}


// ============================================================
// UPDATE
// ============================================================
//
// This currently handles the continuous player movement.
//
// IMPORTANT:
// The camera is NOT connected to these controls.
//
// W / A / S / D and arrow keys affect the player only.
//
// ============================================================

void Game::update(double deltaTime)
{
    constexpr double speed = 250.0;


    // --------------------------------------------------------
    // UP
    // --------------------------------------------------------

    if (keyState[GLFW_KEY_W] ||
        keyState[GLFW_KEY_UP])
    {
        playerY -=
            speed * deltaTime;
    }


    // --------------------------------------------------------
    // DOWN
    // --------------------------------------------------------

    if (keyState[GLFW_KEY_S] ||
        keyState[GLFW_KEY_DOWN])
    {
        playerY +=
            speed * deltaTime;
    }


    // --------------------------------------------------------
    // LEFT
    // --------------------------------------------------------

    if (keyState[GLFW_KEY_A] ||
        keyState[GLFW_KEY_LEFT])
    {
        playerX -=
            speed * deltaTime;
    }


    // --------------------------------------------------------
    // RIGHT
    // --------------------------------------------------------

    if (keyState[GLFW_KEY_D] ||
        keyState[GLFW_KEY_RIGHT])
    {
        playerX +=
            speed * deltaTime;
    }


    // --------------------------------------------------------
    // TEMPORARY LOGICAL WORLD BOUNDS
    // --------------------------------------------------------

    if (playerX < 0.0)
        playerX = 0.0;


    if (playerX >
        Renderer::LOGICAL_WIDTH)
    {
        playerX =
            Renderer::LOGICAL_WIDTH;
    }


    if (playerY < 0.0)
        playerY = 0.0;


    if (playerY >
        Renderer::LOGICAL_HEIGHT)
    {
        playerY =
            Renderer::LOGICAL_HEIGHT;
    }
}


// ============================================================
// RENDER
// ============================================================

void Game::render()
{
    renderer->beginFrame();


    // ========================================================
    // ROOM 1
    // ========================================================

    renderer->drawRoom();


    // ========================================================
    // PLAYER
    // ========================================================
    //
    // The old temporary square is intentionally removed.
    //
    // We will replace it with the final player design using
    // our CG algorithms in the next stage.
    //
    // ========================================================


    renderer->endFrame();
}