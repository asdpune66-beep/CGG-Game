#pragma once

#include <GLFW/glfw3.h>

class Renderer
{
public:
    // =========================================================
    // LOGICAL GAME RESOLUTION
    // =========================================================

    static constexpr float LOGICAL_WIDTH  = 1280.0f;
    static constexpr float LOGICAL_HEIGHT = 720.0f;


    // =========================================================
    // ROOM 1 LAYOUT
    // =========================================================

    static constexpr float ROOM_LEFT   = 65.0f;
    static constexpr float ROOM_RIGHT  = 1215.0f;
    static constexpr float ROOM_TOP    = 95.0f;
    static constexpr float ROOM_BOTTOM = 650.0f;

    static constexpr float ENTRY_LEFT  = 520.0f;
    static constexpr float ENTRY_RIGHT = 760.0f;


    // =========================================================
    // CONSTRUCTOR
    // =========================================================

    explicit Renderer(GLFWwindow* window);


    // =========================================================
    // FRAME
    // =========================================================

    void beginFrame();
    void endFrame();


    // =========================================================
    // WINDOW RESIZING
    // =========================================================

    void resize(
        int framebufferWidth,
        int framebufferHeight
    );


    // =========================================================
    // ROOM RENDERING
    // =========================================================

    void drawRoom();


private:

    GLFWwindow* window;

    int framebufferWidth  = 1280;
    int framebufferHeight = 720;


    // =========================================================
    // VIEWPORT
    // =========================================================

    void updateViewport();


    // =========================================================
    // FUNDAMENTAL COMPUTER GRAPHICS ALGORITHMS
    // =========================================================

    // DDA line drawing
    void drawDDALine(
        float x1,
        float y1,
        float x2,
        float y2
    );

        void drawMidpointCircle(
        int centerX,
        int centerY,
        int radius
    );


    // Scanline colour filling
    void drawScanlineFilledRectangle(
        float left,
        float top,
        float right,
        float bottom
    );


    // Filled rectangle with DDA outline
    void drawFilledRectangle(
        float left,
        float top,
        float right,
        float bottom
    );


    // =========================================================
    // ROOM 1 OBJECTS
    // =========================================================

    void drawReceptionDesk();

    void drawComputer();

    void drawChair();

    void drawNoticeBoard();

    void drawCabinet();

    void drawCrate();

    void drawTable();

    void drawPlant();

    void drawCCTV();
};