#include "Renderer.h"

#include <algorithm>
#include <cmath>
#include <cstring>


// =============================================================
// CONSTRUCTOR
// =============================================================

Renderer::Renderer(GLFWwindow* window)
    : window(window)
{
    glfwGetFramebufferSize( 
        window,
        &framebufferWidth,
        &framebufferHeight
    );

    updateViewport();
}


// =============================================================
// WINDOW RESIZING
// =============================================================

void Renderer::resize(
    int width,
    int height
)
{
    framebufferWidth =
        std::max(width, 1);

    framebufferHeight =
        std::max(height, 1);

    updateViewport();
}


// =============================================================
// VIEWPORT
// =============================================================

void Renderer::updateViewport()
{
    const float logicalAspect =
        LOGICAL_WIDTH / LOGICAL_HEIGHT;

    const float screenAspect =
        static_cast<float>(framebufferWidth) /
        static_cast<float>(framebufferHeight);

    int viewportWidth;
    int viewportHeight;


    if (screenAspect > logicalAspect)
    {
        viewportHeight =
            framebufferHeight;

        viewportWidth =
            static_cast<int>(
                viewportHeight * logicalAspect
            );
    }
    else
    {
        viewportWidth =
            framebufferWidth;

        viewportHeight =
            static_cast<int>(
                viewportWidth / logicalAspect
            );
    }


    const int viewportX =
        (framebufferWidth - viewportWidth) / 2;

    const int viewportY =
        (framebufferHeight - viewportHeight) / 2;


    glViewport(
        viewportX,
        viewportY,
        viewportWidth,
        viewportHeight
    );
}


// =============================================================
// BEGIN FRAME
// =============================================================

void Renderer::beginFrame()
{
    glClearColor(
        0.02f,
        0.025f,
        0.03f,
        1.0f
    );

    glClear(
        GL_COLOR_BUFFER_BIT
    );


    glMatrixMode(
        GL_PROJECTION
    );

    glLoadIdentity();


    glOrtho(
        0.0,
        LOGICAL_WIDTH,
        LOGICAL_HEIGHT,
        0.0,
        -1.0,
        1.0
    );


    glMatrixMode(
        GL_MODELVIEW
    );

    glLoadIdentity();
}


// =============================================================
// END FRAME
// =============================================================

void Renderer::endFrame()
{
    glfwSwapBuffers(
        window
    );
}


// =============================================================
// DDA LINE DRAWING ALGORITHM
// =============================================================

void Renderer::drawDDALine(
    float x1,
    float y1,
    float x2,
    float y2
)
{
    const float dx =
        x2 - x1;

    const float dy =
        y2 - y1;


    const float steps =
        std::max(
            std::abs(dx),
            std::abs(dy)
        );


    if (steps == 0.0f)
    {
        glBegin(GL_POINTS);

        glVertex2f(
            x1,
            y1
        );

        glEnd();

        return;
    }


    const float xIncrement =
        dx / steps;

    const float yIncrement =
        dy / steps;


    float x = x1;
    float y = y1;


    glBegin(GL_POINTS);


    for (
        int i = 0;
        i <= static_cast<int>(steps);
        ++i
    )
    {
        glVertex2f(
            std::round(x),
            std::round(y)
        );

        x += xIncrement;
        y += yIncrement;
    }


    glEnd();
}

// =============================================================
// MIDPOINT CIRCLE ALGORITHM
// =============================================================

void Renderer::drawMidpointCircle(
    int centerX,
    int centerY,
    int radius
)
{
    int x = 0;
    int y = radius;

    int decisionParameter = 1 - radius;

    glBegin(GL_POINTS);

    while (x <= y)
    {
        glVertex2i(centerX + x, centerY + y);
        glVertex2i(centerX - x, centerY + y);
        glVertex2i(centerX + x, centerY - y);
        glVertex2i(centerX - x, centerY - y);

        glVertex2i(centerX + y, centerY + x);
        glVertex2i(centerX - y, centerY + x);
        glVertex2i(centerX + y, centerY - x);
        glVertex2i(centerX - y, centerY - x);

        if (decisionParameter < 0)
        {
            decisionParameter += 2 * x + 3;
        }
        else
        {
            decisionParameter += 2 * (x - y) + 5;
            y--;
        }

        x++;
    }

    glEnd();
}


// =============================================================
// SCANLINE COLOUR FILLING
// =============================================================

void Renderer::drawScanlineFilledRectangle(
    float left,
    float top,
    float right,
    float bottom
)
{
    const int startY =
        static_cast<int>(
            std::ceil(top)
        );

    const int endY =
        static_cast<int>(
            std::floor(bottom)
        );


    glBegin(GL_POINTS);


    for (
        int y = startY;
        y <= endY;
        ++y
    )
    {
        const int startX =
            static_cast<int>(
                std::ceil(left)
            );

        const int endX =
            static_cast<int>(
                std::floor(right)
            );


        for (
            int x = startX;
            x <= endX;
            ++x
        )
        {
            glVertex2i(
                x,
                y
            );
        }
    }


    glEnd();
}


// =============================================================
// FILLED RECTANGLE + DDA OUTLINE
// =============================================================

void Renderer::drawFilledRectangle(
    float left,
    float top,
    float right,
    float bottom
)
{
    drawScanlineFilledRectangle(
        left,
        top,
        right,
        bottom
    );


    glColor3f(
        0.75f,
        0.78f,
        0.82f
    );


    drawDDALine(
        left,
        top,
        right,
        top
    );

    drawDDALine(
        right,
        top,
        right,
        bottom
    );

    drawDDALine(
        right,
        bottom,
        left,
        bottom
    );

    drawDDALine(
        left,
        bottom,
        left,
        top
    );
}


// =============================================================
// ROOM 1 — RECEPTION DESK
// =============================================================

void Renderer::drawReceptionDesk()
{
    // ---------------------------------------------------------
    // MAIN DESK TOP
    // ---------------------------------------------------------

    glColor3f(
        0.34f,
        0.20f,
        0.09f
    );

    drawScanlineFilledRectangle(
        145.0f,
        350.0f,
        385.0f,
        405.0f
    );


    // ---------------------------------------------------------
    // LEFT SIDE / REAR SECTION
    // ---------------------------------------------------------

    glColor3f(
        0.28f,
        0.16f,
        0.07f
    );

    drawScanlineFilledRectangle(
        145.0f,
        350.0f,
        185.0f,
        430.0f
    );


    // ---------------------------------------------------------
    // RIGHT RETURN / SIDE WING
    // ---------------------------------------------------------

    glColor3f(
        0.30f,
        0.18f,
        0.08f
    );

    drawScanlineFilledRectangle(
        340.0f,
        305.0f,
        385.0f,
        430.0f
    );


    // ---------------------------------------------------------
    // FRONT PANEL
    // ---------------------------------------------------------

    glColor3f(
        0.20f,
        0.12f,
        0.06f
    );

    drawScanlineFilledRectangle(
        185.0f,
        385.0f,
        340.0f,
        430.0f
    );


    // ---------------------------------------------------------
    // INNER FRONT PANEL
    // ---------------------------------------------------------

    glColor3f(
        0.24f,
        0.14f,
        0.06f
    );

    drawScanlineFilledRectangle(
        205.0f,
        398.0f,
        320.0f,
        420.0f
    );


    // ---------------------------------------------------------
    // DESK OUTLINES
    // ---------------------------------------------------------

    glColor3f(
        0.78f,
        0.55f,
        0.28f
    );


    drawDDALine(
        145.0f,
        350.0f,
        385.0f,
        350.0f
    );

    drawDDALine(
        145.0f,
        350.0f,
        145.0f,
        430.0f
    );

    drawDDALine(
        145.0f,
        430.0f,
        185.0f,
        430.0f
    );

    drawDDALine(
        185.0f,
        430.0f,
        340.0f,
        430.0f
    );

    drawDDALine(
        340.0f,
        430.0f,
        385.0f,
        430.0f
    );

    drawDDALine(
        385.0f,
        430.0f,
        385.0f,
        305.0f
    );

    drawDDALine(
        385.0f,
        305.0f,
        340.0f,
        305.0f
    );

    drawDDALine(
        340.0f,
        305.0f,
        340.0f,
        350.0f
    );


    // Front panel
    drawDDALine(
        185.0f,
        385.0f,
        340.0f,
        385.0f
    );

    drawDDALine(
        185.0f,
        385.0f,
        185.0f,
        430.0f
    );

    drawDDALine(
        340.0f,
        385.0f,
        340.0f,
        430.0f
    );


    // Inner panel
    drawDDALine(
        205.0f,
        398.0f,
        320.0f,
        398.0f
    );

    drawDDALine(
        320.0f,
        398.0f,
        320.0f,
        420.0f
    );

    drawDDALine(
        320.0f,
        420.0f,
        205.0f,
        420.0f
    );

    drawDDALine(
        205.0f,
        420.0f,
        205.0f,
        398.0f
    );
}


// =============================================================
// ROOM 1 — CLEARLY IDENTIFIABLE COMPUTER
// =============================================================

void Renderer::drawComputer()
{
    // =========================================================
    // MONITOR OUTER BODY
    // =========================================================

    glColor3f(
        0.08f,
        0.11f,
        0.15f
    );

    drawScanlineFilledRectangle(
        185.0f,
        235.0f,
        330.0f,
        315.0f
    );


    // =========================================================
    // MONITOR BEZEL
    // =========================================================

    glColor3f(
        0.22f,
        0.25f,
        0.29f
    );

    drawScanlineFilledRectangle(
        192.0f,
        242.0f,
        323.0f,
        308.0f
    );


    // =========================================================
    // SCREEN
    // =========================================================

    glColor3f(
        0.015f,
        0.12f,
        0.16f
    );

    drawScanlineFilledRectangle(
        202.0f,
        251.0f,
        313.0f,
        299.0f
    );


    // =========================================================
    // SCREEN INNER AREA
    // =========================================================

    glColor3f(
        0.025f,
        0.18f,
        0.22f
    );

    drawScanlineFilledRectangle(
        208.0f,
        257.0f,
        307.0f,
        293.0f
    );


    // =========================================================
    // SCREEN UI — TOP BAR
    // =========================================================

    glColor3f(
        0.18f,
        0.55f,
        0.62f
    );

    drawScanlineFilledRectangle(
        214.0f,
        262.0f,
        301.0f,
        266.0f
    );


    // =========================================================
    // SCREEN UI — TEXT / TERMINAL LINES
    // =========================================================

    glColor3f(
        0.35f,
        0.72f,
        0.72f
    );

    drawDDALine(
        216.0f,
        273.0f,
        267.0f,
        273.0f
    );

    drawDDALine(
        216.0f,
        280.0f,
        288.0f,
        280.0f
    );

    drawDDALine(
        216.0f,
        287.0f,
        254.0f,
        287.0f
    );


    // =========================================================
    // CURSOR
    // =========================================================

    drawScanlineFilledRectangle(
        260.0f,
        286.0f,
        265.0f,
        291.0f
    );


    // =========================================================
    // POWER INDICATOR
    // =========================================================

    glColor3f(
        0.20f,
        0.90f,
        0.55f
    );

    drawScanlineFilledRectangle(
        304.0f,
        302.0f,
        311.0f,
        305.0f
    );


    // =========================================================
    // MONITOR STAND
    // =========================================================

    glColor3f(
        0.14f,
        0.17f,
        0.21f
    );

    drawScanlineFilledRectangle(
        247.0f,
        315.0f,
        269.0f,
        331.0f
    );


    // =========================================================
    // MONITOR BASE
    // =========================================================

    glColor3f(
        0.20f,
        0.23f,
        0.27f
    );

    drawScanlineFilledRectangle(
        225.0f,
        330.0f,
        291.0f,
        337.0f
    );


    // =========================================================
    // CPU TOWER
    // =========================================================

    glColor3f(
        0.12f,
        0.15f,
        0.19f
    );

    drawScanlineFilledRectangle(
        330.0f,
        300.0f,
        365.0f,
        350.0f
    );


    // CPU front panel
    glColor3f(
        0.18f,
        0.21f,
        0.25f
    );

    drawScanlineFilledRectangle(
        336.0f,
        306.0f,
        359.0f,
        345.0f
    );


    // CPU drive bay
    glColor3f(
        0.08f,
        0.10f,
        0.13f
    );

    drawScanlineFilledRectangle(
        340.0f,
        312.0f,
        355.0f,
        317.0f
    );


    // CPU power button
    glColor3f(
        0.20f,
        0.80f,
        0.55f
    );

    drawScanlineFilledRectangle(
        344.0f,
        326.0f,
        351.0f,
        330.0f
    );


    // =========================================================
    // KEYBOARD BODY
    // =========================================================

    glColor3f(
        0.16f,
        0.19f,
        0.23f
    );

    drawScanlineFilledRectangle(
        190.0f,
        340.0f,
        300.0f,
        357.0f
    );


    // =========================================================
    // KEYBOARD KEYS — ROW 1
    // =========================================================

    glColor3f(
        0.42f,
        0.46f,
        0.50f
    );

    for (
        int x = 197;
        x <= 285;
        x += 11
    )
    {
        drawScanlineFilledRectangle(
            static_cast<float>(x),
            343.0f,
            static_cast<float>(x + 7),
            347.0f
        );
    }


    // =========================================================
    // KEYBOARD KEYS — ROW 2
    // =========================================================

    for (
        int x = 200;
        x <= 282;
        x += 11
    )
    {
        drawScanlineFilledRectangle(
            static_cast<float>(x),
            349.0f,
            static_cast<float>(x + 7),
            353.0f
        );
    }


    // =========================================================
    // SPACE BAR
    // =========================================================

    drawScanlineFilledRectangle(
        224.0f,
        354.0f,
        267.0f,
        356.0f
    );


    // =========================================================
    // MOUSE PAD
    // =========================================================

    glColor3f(
        0.07f,
        0.09f,
        0.12f
    );

    drawScanlineFilledRectangle(
        305.0f,
        340.0f,
        330.0f,
        360.0f
    );


    // =========================================================
    // MOUSE
    // =========================================================

    glColor3f(
        0.25f,
        0.28f,
        0.32f
    );

    drawScanlineFilledRectangle(
        309.0f,
        344.0f,
        326.0f,
        357.0f
    );


    // Mouse center line
    glColor3f(
        0.60f,
        0.64f,
        0.68f
    );

    drawDDALine(
        317.0f,
        345.0f,
        317.0f,
        356.0f
    );


    // =========================================================
    // MONITOR OUTLINE
    // =========================================================

    glColor3f(
        0.75f,
        0.80f,
        0.84f
    );

    drawDDALine(
        185.0f,
        235.0f,
        330.0f,
        235.0f
    );

    drawDDALine(
        330.0f,
        235.0f,
        330.0f,
        315.0f
    );

    drawDDALine(
        330.0f,
        315.0f,
        185.0f,
        315.0f
    );

    drawDDALine(
        185.0f,
        315.0f,
        185.0f,
        235.0f
    );


    // =========================================================
    // SCREEN OUTLINE
    // =========================================================

    glColor3f(
        0.35f,
        0.70f,
        0.74f
    );

    drawDDALine(
        202.0f,
        251.0f,
        313.0f,
        251.0f
    );

    drawDDALine(
        313.0f,
        251.0f,
        313.0f,
        299.0f
    );

    drawDDALine(
        313.0f,
        299.0f,
        202.0f,
        299.0f
    );

    drawDDALine(
        202.0f,
        299.0f,
        202.0f,
        251.0f
    );


    // =========================================================
    // CPU OUTLINE
    // =========================================================

    glColor3f(
        0.62f,
        0.67f,
        0.72f
    );

    drawDDALine(
        330.0f,
        300.0f,
        365.0f,
        300.0f
    );

    drawDDALine(
        365.0f,
        300.0f,
        365.0f,
        350.0f
    );

    drawDDALine(
        365.0f,
        350.0f,
        330.0f,
        350.0f
    );

    drawDDALine(
        330.0f,
        350.0f,
        330.0f,
        300.0f
    );


    // =========================================================
    // KEYBOARD OUTLINE
    // =========================================================

    drawDDALine(
        190.0f,
        340.0f,
        300.0f,
        340.0f
    );

    drawDDALine(
        300.0f,
        340.0f,
        300.0f,
        357.0f
    );

    drawDDALine(
        300.0f,
        357.0f,
        190.0f,
        357.0f
    );

    drawDDALine(
        190.0f,
        357.0f,
        190.0f,
        340.0f
    );


    // =========================================================
    // MOUSE PAD OUTLINE
    // =========================================================

    drawDDALine(
        305.0f,
        340.0f,
        330.0f,
        340.0f
    );

    drawDDALine(
        330.0f,
        340.0f,
        330.0f,
        360.0f
    );

    drawDDALine(
        330.0f,
        360.0f,
        305.0f,
        360.0f
    );

    drawDDALine(
        305.0f,
        360.0f,
        305.0f,
        340.0f
    );
}


// =============================================================
// ROOM 1 — RECEPTION CHAIR
// =============================================================

void Renderer::drawChair()
{
    // Chair back
    glColor3f(
        0.10f,
        0.12f,
        0.15f
    );

    drawScanlineFilledRectangle(
        235.0f,
        300.0f,
        300.0f,
        345.0f
    );


    // Chair seat
    glColor3f(
        0.13f,
        0.15f,
        0.18f
    );

    drawScanlineFilledRectangle(
        220.0f,
        340.0f,
        315.0f,
        360.0f
    );


    // Arm supports
    drawScanlineFilledRectangle(
        215.0f,
        340.0f,
        225.0f,
        365.0f
    );

    drawScanlineFilledRectangle(
        310.0f,
        340.0f,
        320.0f,
        365.0f
    );


    // Outline
    glColor3f(
        0.55f,
        0.59f,
        0.64f
    );

    drawDDALine(
        235.0f,
        300.0f,
        300.0f,
        300.0f
    );

    drawDDALine(
        300.0f,
        300.0f,
        300.0f,
        345.0f
    );

    drawDDALine(
        300.0f,
        345.0f,
        315.0f,
        345.0f
    );

    drawDDALine(
        315.0f,
        345.0f,
        315.0f,
        360.0f
    );

    drawDDALine(
        315.0f,
        360.0f,
        220.0f,
        360.0f
    );

    drawDDALine(
        220.0f,
        360.0f,
        220.0f,
        340.0f
    );

    drawDDALine(
        220.0f,
        340.0f,
        235.0f,
        340.0f
    );

    drawDDALine(
        235.0f,
        340.0f,
        235.0f,
        300.0f
    );
}


// =============================================================
// ROOM 1 — NOTICE BOARD
// =============================================================

void Renderer::drawNoticeBoard()
{
    // ---------------------------------------------------------
    // OUTER FRAME
    // ---------------------------------------------------------

    glColor3f(
        0.18f,
        0.12f,
        0.08f
    );

    drawScanlineFilledRectangle(
        500.0f,
        150.0f,
        700.0f,
        260.0f
    );


    // ---------------------------------------------------------
    // INNER BOARD
    // ---------------------------------------------------------

    glColor3f(
        0.24f,
        0.28f,
        0.25f
    );

    drawScanlineFilledRectangle(
        512.0f,
        162.0f,
        688.0f,
        248.0f
    );


    // ---------------------------------------------------------
    // NOTICE 1
    // ---------------------------------------------------------

    glColor3f(
        0.72f,
        0.70f,
        0.60f
    );

    drawScanlineFilledRectangle(
        530.0f,
        180.0f,
        580.0f,
        220.0f
    );


    // ---------------------------------------------------------
    // NOTICE 2
    // ---------------------------------------------------------

    glColor3f(
        0.62f,
        0.68f,
        0.70f
    );

    drawScanlineFilledRectangle(
        605.0f,
        175.0f,
        660.0f,
        215.0f
    );


    // ---------------------------------------------------------
    // NOTICE 3
    // ---------------------------------------------------------

    glColor3f(
        0.70f,
        0.62f,
        0.55f
    );

    drawScanlineFilledRectangle(
        665.0f,
        185.0f,
        680.0f,
        230.0f
    );


    // ---------------------------------------------------------
    // NOTICE TEXT LINES
    // ---------------------------------------------------------

    glColor3f(
        0.25f,
        0.28f,
        0.28f
    );

    drawDDALine(
        538.0f,
        190.0f,
        570.0f,
        190.0f
    );

    drawDDALine(
        538.0f,
        198.0f,
        568.0f,
        198.0f
    );

    drawDDALine(
        538.0f,
        206.0f,
        560.0f,
        206.0f
    );


    drawDDALine(
        613.0f,
        185.0f,
        650.0f,
        185.0f
    );

    drawDDALine(
        613.0f,
        193.0f,
        645.0f,
        193.0f
    );

    drawDDALine(
        613.0f,
        201.0f,
        638.0f,
        201.0f
    );


    // ---------------------------------------------------------
    // BOARD OUTLINE
    // ---------------------------------------------------------

    glColor3f(
        0.72f,
        0.68f,
        0.55f
    );

    drawDDALine(
        500.0f,
        150.0f,
        700.0f,
        150.0f
    );

    drawDDALine(
        700.0f,
        150.0f,
        700.0f,
        260.0f
    );

    drawDDALine(
        700.0f,
        260.0f,
        500.0f,
        260.0f
    );

    drawDDALine(
        500.0f,
        260.0f,
        500.0f,
        150.0f
    );


    // Inner frame
    drawDDALine(
        512.0f,
        162.0f,
        688.0f,
        162.0f
    );

    drawDDALine(
        688.0f,
        162.0f,
        688.0f,
        248.0f
    );

    drawDDALine(
        688.0f,
        248.0f,
        512.0f,
        248.0f
    );

    drawDDALine(
        512.0f,
        248.0f,
        512.0f,
        162.0f
    );

}// =============================================================
// ROOM 1 — FILING CABINET
// =============================================================

void Renderer::drawCabinet()
{
    // ---------------------------------------------------------
    // MAIN CABINET BODY
    // ---------------------------------------------------------

    glColor3f(
        0.18f,
        0.21f,
        0.25f
    );

    drawScanlineFilledRectangle(
        930.0f,
        245.0f,
        1035.0f,
        390.0f
    );


    // ---------------------------------------------------------
    // INNER FRONT PANEL
    // ---------------------------------------------------------

    glColor3f(
        0.24f,
        0.27f,
        0.31f
    );

    drawScanlineFilledRectangle(
        940.0f,
        255.0f,
        1025.0f,
        380.0f
    );


    // ---------------------------------------------------------
    // DRAWER 1
    // ---------------------------------------------------------

    glColor3f(
        0.28f,
        0.31f,
        0.35f
    );

    drawScanlineFilledRectangle(
        946.0f,
        262.0f,
        1019.0f,
        292.0f
    );


    // ---------------------------------------------------------
    // DRAWER 2
    // ---------------------------------------------------------

    drawScanlineFilledRectangle(
        946.0f,
        299.0f,
        1019.0f,
        329.0f
    );


    // ---------------------------------------------------------
    // DRAWER 3
    // ---------------------------------------------------------

    drawScanlineFilledRectangle(
        946.0f,
        336.0f,
        1019.0f,
        366.0f
    );


    // ---------------------------------------------------------
    // DRAWER HANDLES
    // ---------------------------------------------------------

    glColor3f(
        0.72f,
        0.75f,
        0.78f
    );


    // Handle 1
    drawScanlineFilledRectangle(
        975.0f,
        276.0f,
        990.0f,
        280.0f
    );


    // Handle 2
    drawScanlineFilledRectangle(
        975.0f,
        313.0f,
        990.0f,
        317.0f
    );


    // Handle 3
    drawScanlineFilledRectangle(
        975.0f,
        350.0f,
        990.0f,
        354.0f
    );


    // ---------------------------------------------------------
    // CABINET OUTLINE
    // ---------------------------------------------------------

    glColor3f(
        0.68f,
        0.73f,
        0.78f
    );


    drawDDALine(
        930.0f,
        245.0f,
        1035.0f,
        245.0f
    );

    drawDDALine(
        1035.0f,
        245.0f,
        1035.0f,
        390.0f
    );

    drawDDALine(
        1035.0f,
        390.0f,
        930.0f,
        390.0f
    );

    drawDDALine(
        930.0f,
        390.0f,
        930.0f,
        245.0f
    );


    // ---------------------------------------------------------
    // DRAWER SEPARATORS
    // ---------------------------------------------------------

    drawDDALine(
        940.0f,
        296.0f,
        1025.0f,
        296.0f
    );

    drawDDALine(
        940.0f,
        333.0f,
        1025.0f,
        333.0f
    );


    // ---------------------------------------------------------
    // DRAWER OUTLINES
    // ---------------------------------------------------------

    drawDDALine(
        946.0f,
        262.0f,
        1019.0f,
        262.0f
    );

    drawDDALine(
        946.0f,
        292.0f,
        1019.0f,
        292.0f
    );


    drawDDALine(
        946.0f,
        299.0f,
        1019.0f,
        299.0f
    );

    drawDDALine(
        946.0f,
        329.0f,
        1019.0f,
        329.0f
    );


    drawDDALine(
        946.0f,
        336.0f,
        1019.0f,
        336.0f
    );

    drawDDALine(
        946.0f,
        366.0f,
        1019.0f,
        366.0f
    );
}

// =============================================================
// ROOM 1 — FILING CABINET
// =============================================================
// =============================================================
// ROOM 1 — CRATE
// =============================================================

// =============================================================
// ROOM 1 — CRATE
// =============================================================

// =============================================================
// ROOM 1 — CRATE
// =============================================================

void Renderer::drawCrate()
{
    // ---------------------------------------------------------
    // CRATE POSITION
    // Bottom-right corner of the room
    // ---------------------------------------------------------

    const float left   = 1135.0f;
    const float top    = 560.0f;
    const float right  = 1205.0f;
    const float bottom = 635.0f;


    // ---------------------------------------------------------
    // OUTER CRATE BODY
    // ---------------------------------------------------------

    glColor3f(
        0.30f,
        0.18f,
        0.07f
    );

    drawScanlineFilledRectangle(
        left,
        top,
        right,
        bottom
    );


    // ---------------------------------------------------------
    // INNER WOODEN PANEL
    // ---------------------------------------------------------

    glColor3f(
        0.38f,
        0.23f,
        0.09f
    );

    drawScanlineFilledRectangle(
        left + 6.0f,
        top + 6.0f,
        right - 6.0f,
        bottom - 6.0f
    );


    // ---------------------------------------------------------
    // INNER BORDER
    // ---------------------------------------------------------

    glColor3f(
        0.75f,
        0.50f,
        0.20f
    );

    drawDDALine(
        left + 6.0f,
        top + 6.0f,
        right - 6.0f,
        top + 6.0f
    );

    drawDDALine(
        right - 6.0f,
        top + 6.0f,
        right - 6.0f,
        bottom - 6.0f
    );

    drawDDALine(
        right - 6.0f,
        bottom - 6.0f,
        left + 6.0f,
        bottom - 6.0f
    );

    drawDDALine(
        left + 6.0f,
        bottom - 6.0f,
        left + 6.0f,
        top + 6.0f
    );


    // ---------------------------------------------------------
    // FIRST DIAGONAL
    // ---------------------------------------------------------

    glColor3f(
        0.68f,
        0.43f,
        0.16f
    );

    drawDDALine(
        left + 8.0f,
        top + 8.0f,
        right - 8.0f,
        bottom - 8.0f
    );


    // ---------------------------------------------------------
    // SECOND DIAGONAL
    // ---------------------------------------------------------

    drawDDALine(
        right - 8.0f,
        top + 8.0f,
        left + 8.0f,
        bottom - 8.0f
    );


    // ---------------------------------------------------------
    // OUTER CRATE OUTLINE
    // ---------------------------------------------------------

    glColor3f(
        0.82f,
        0.57f,
        0.24f
    );

    drawDDALine(
        left,
        top,
        right,
        top
    );

    drawDDALine(
        right,
        top,
        right,
        bottom
    );

    drawDDALine(
        right,
        bottom,
        left,
        bottom
    );

    drawDDALine(
        left,
        bottom,
        left,
        top
    );
}

// =============================================================
// ROOM 1
// =============================================================

// =============================================================
// ROOM 1 — TABLE
// =============================================================

void Renderer::drawTable()
{
    // ---------------------------------------------------------
    // TABLE POSITION
    // Lower-middle area of the room
    // ---------------------------------------------------------

    const float left   = 560.0f;
    const float top    = 470.0f;
    const float right  = 780.0f;
    const float bottom = 550.0f;


    // ---------------------------------------------------------
    // TABLE TOP
    // ---------------------------------------------------------

    glColor3f(
        0.32f,
        0.19f,
        0.08f
    );

    drawScanlineFilledRectangle(
        left,
        top,
        right,
        bottom
    );


    // ---------------------------------------------------------
    // INNER TABLE SURFACE
    // ---------------------------------------------------------

    glColor3f(
        0.42f,
        0.25f,
        0.10f
    );

    drawScanlineFilledRectangle(
        left + 8.0f,
        top + 8.0f,
        right - 8.0f,
        bottom - 8.0f
    );


    // ---------------------------------------------------------
    // TABLE INNER BORDER
    // ---------------------------------------------------------

    glColor3f(
        0.70f,
        0.44f,
        0.17f
    );

    drawDDALine(
        left + 8.0f,
        top + 8.0f,
        right - 8.0f,
        top + 8.0f
    );

    drawDDALine(
        right - 8.0f,
        top + 8.0f,
        right - 8.0f,
        bottom - 8.0f
    );

    drawDDALine(
        right - 8.0f,
        bottom - 8.0f,
        left + 8.0f,
        bottom - 8.0f
    );

    drawDDALine(
        left + 8.0f,
        bottom - 8.0f,
        left + 8.0f,
        top + 8.0f
    );


    // ---------------------------------------------------------
    // TABLE FRONT EDGE
    // ---------------------------------------------------------

    glColor3f(
        0.25f,
        0.14f,
        0.05f
    );

    drawScanlineFilledRectangle(
        left + 15.0f,
        bottom - 12.0f,
        right - 15.0f,
        bottom - 3.0f
    );


    // ---------------------------------------------------------
    // TABLE LEGS
    // ---------------------------------------------------------

    glColor3f(
        0.24f,
        0.14f,
        0.06f
    );

    // Left leg
    drawScanlineFilledRectangle(
        left + 18.0f,
        bottom - 3.0f,
        left + 30.0f,
        bottom + 18.0f
    );

    // Right leg
    drawScanlineFilledRectangle(
        right - 30.0f,
        bottom - 3.0f,
        right - 18.0f,
        bottom + 18.0f
    );


    // ---------------------------------------------------------
    // TABLE OUTLINE
    // ---------------------------------------------------------

    glColor3f(
        0.80f,
        0.52f,
        0.20f
    );

    drawDDALine(
        left,
        top,
        right,
        top
    );

    drawDDALine(
        right,
        top,
        right,
        bottom
    );

    drawDDALine(
        right,
        bottom,
        left,
        bottom
    );

    drawDDALine(
        left,
        bottom,
        left,
        top
    );
}

// =============================================================
// ROOM 1 — PLANT
// =============================================================

// =============================================================
// ROOM 1 — SMALL CORNER PLANT
// =============================================================

// void Renderer::drawPlant()
// {
//     // ---------------------------------------------------------
//     // PLANT POSITION
//     // Upper-left corner of the room
//     // ---------------------------------------------------------

//     const float centerX = 105.0f;
//     const float potTop  = 170.0f;


//     // ---------------------------------------------------------
//     // STEM
//     // ---------------------------------------------------------

//     glColor3f(
//         0.18f,
//         0.48f,
//         0.18f
//     );

//     drawDDALine(
//         centerX,
//         145.0f,
//         centerX,
//         potTop
//     );


//     // ---------------------------------------------------------
//     // LEFT LEAF
//     // ---------------------------------------------------------

//     glColor3f(
//         0.12f,
//         0.55f,
//         0.18f
//     );

//     glBegin(GL_POLYGON);

//     glVertex2f(centerX, 150.0f);
//     glVertex2f(centerX - 18.0f, 138.0f);
//     glVertex2f(centerX - 27.0f, 120.0f);
//     glVertex2f(centerX - 10.0f, 125.0f);
//     glVertex2f(centerX, 140.0f);

//     glEnd();


//     // ---------------------------------------------------------
//     // RIGHT LEAF
//     // ---------------------------------------------------------

//     glColor3f(
//         0.16f,
//         0.62f,
//         0.20f
//     );

//     glBegin(GL_POLYGON);

//     glVertex2f(centerX, 150.0f);
//     glVertex2f(centerX + 18.0f, 138.0f);
//     glVertex2f(centerX + 27.0f, 120.0f);
//     glVertex2f(centerX + 10.0f, 125.0f);
//     glVertex2f(centerX, 140.0f);

//     glEnd();


//     // ---------------------------------------------------------
//     // TOP LEAF
//     // ---------------------------------------------------------

//     glColor3f(
//         0.18f,
//         0.68f,
//         0.22f
//     );

//     glBegin(GL_POLYGON);

//     glVertex2f(centerX, 145.0f);
//     glVertex2f(centerX - 11.0f, 130.0f);
//     glVertex2f(centerX - 8.0f, 108.0f);
//     glVertex2f(centerX, 118.0f);
//     glVertex2f(centerX + 8.0f, 108.0f);
//     glVertex2f(centerX + 11.0f, 130.0f);

//     glEnd();


//     // ---------------------------------------------------------
//     // LEFT LOWER LEAF
//     // ---------------------------------------------------------

//     glColor3f(
//         0.10f,
//         0.48f,
//         0.15f
//     );

//     glBegin(GL_POLYGON);

//     glVertex2f(centerX - 2.0f, 150.0f);
//     glVertex2f(centerX - 24.0f, 146.0f);
//     glVertex2f(centerX - 30.0f, 132.0f);
//     glVertex2f(centerX - 12.0f, 137.0f);

//     glEnd();


//     // ---------------------------------------------------------
//     // RIGHT LOWER LEAF
//     // ---------------------------------------------------------

//     glColor3f(
//         0.13f,
//         0.58f,
//         0.18f
//     );

//     glBegin(GL_POLYGON);

//     glVertex2f(centerX + 2.0f, 150.0f);
//     glVertex2f(centerX + 24.0f, 146.0f);
//     glVertex2f(centerX + 30.0f, 132.0f);
//     glVertex2f(centerX + 12.0f, 137.0f);

//     glEnd();


//     // ---------------------------------------------------------
//     // POT
//     // ---------------------------------------------------------

//     glColor3f(
//         0.32f,
//         0.18f,
//         0.08f
//     );

//     glBegin(GL_POLYGON);

//     glVertex2f(centerX - 14.0f, potTop);
//     glVertex2f(centerX + 14.0f, potTop);
//     glVertex2f(centerX + 10.0f, potTop + 20.0f);
//     glVertex2f(centerX - 10.0f, potTop + 20.0f);

//     glEnd();


//     // ---------------------------------------------------------
//     // POT RIM
//     // ---------------------------------------------------------

//     glColor3f(
//         0.55f,
//         0.32f,
//         0.12f
//     );

//     drawScanlineFilledRectangle(
//         centerX - 17.0f,
//         potTop - 3.0f,
//         centerX + 17.0f,
//         potTop + 3.0f
//     );


//     // ---------------------------------------------------------
//     // POT OUTLINE
//     // ---------------------------------------------------------

//     glColor3f(
//         0.72f,
//         0.45f,
//         0.17f
//     );

//     drawDDALine(
//         centerX - 14.0f,
//         potTop,
//         centerX + 14.0f,
//         potTop
//     );

//     drawDDALine(
//         centerX + 14.0f,
//         potTop,
//         centerX + 10.0f,
//         potTop + 20.0f
//     );

//     drawDDALine(
//         centerX + 10.0f,
//         potTop + 20.0f,
//         centerX - 10.0f,
//         potTop + 20.0f
//     );

//     drawDDALine(
//         centerX - 10.0f,
//         potTop + 20.0f,
//         centerX - 14.0f,
//         potTop
//     );


//     // ---------------------------------------------------------
//     // LEAF VEINS
//     // ---------------------------------------------------------

//     glColor3f(
//         0.35f,
//         0.80f,
//         0.30f
//     );

//     drawDDALine(
//         centerX,
//         143.0f,
//         centerX - 20.0f,
//         125.0f
//     );

//     drawDDALine(
//         centerX,
//         143.0f,
//         centerX + 20.0f,
//         125.0f
//     );

//     drawDDALine(
//         centerX,
//         138.0f,
//         centerX,
//         116.0f
//     );
// }

// =============================================================
// ROOM 1 — CCTV
// =============================================================
// =============================================================
// CCTV SWEEP UPDATE
// =============================================================

void Renderer::updateCCTV(double deltaTime)
{
    const float sweepSpeed = 30.0f;

    cctvAngle +=
        cctvSweepDirection *
        sweepSpeed *
        static_cast<float>(deltaTime);

    // ---------------------------------------------------------
    // CCTV is mounted near the TOP-RIGHT corner.
    //
    // The sweep is restricted to the two directions formed
    // by the adjacent top and right walls.
    //
    // 135 degrees = points down-left
    // 180 degrees = points left
    // 225 degrees = points up-left
    //
    // We use a controlled range so the camera never turns
    // outside the room.
    // ---------------------------------------------------------

    const float minimumAngle = 135.0f;
    const float maximumAngle = 180.0f;

    if (cctvAngle >= maximumAngle)
    {
        cctvAngle = maximumAngle;
        cctvSweepDirection = -1.0f;
    }

    if (cctvAngle <= minimumAngle)
    {
        cctvAngle = minimumAngle;
        cctvSweepDirection = 1.0f;
    }
}

// =============================================================
// ROOM 1 — CCTV
// =============================================================


void Renderer::updateGeneratorAnimation(double deltaTime)
{
    generatorPulseTime += static_cast<float>(deltaTime);

    const float twoPi = 6.28318530718f;

    if (generatorPulseTime >= twoPi)
    {
        generatorPulseTime = std::fmod(generatorPulseTime, twoPi);
    }
}

void Renderer::drawCCTV()
{
    // ---------------------------------------------------------
    // CAMERA POSITION
    // Mounted close to the top-right wall
    // ---------------------------------------------------------

    const float cameraX = 1165.0f;
    const float cameraY = 120.0f;


    // // ---------------------------------------------------------
    // // DETECTION RANGE
    // // Midpoint Circle Algorithm
    // // ---------------------------------------------------------

    // glColor3f(
    //     0.16f,
    //     0.28f,
    //     0.34f
    // );

    // drawMidpointCircle(
    //     static_cast<int>(cameraX),
    //     static_cast<int>(cameraY),
    //     105
    // );


    // ---------------------------------------------------------
    // CAMERA DIRECTION
    // Convert angle to radians
    // ---------------------------------------------------------

    const float radians =
        cctvAngle *
        3.14159265359f /
        180.0f;


    const float directionX =
        std::cos(radians);

    const float directionY =
        std::sin(radians);


    // ---------------------------------------------------------
    // PERPENDICULAR VECTOR
    // Used to create the surveillance cone
    // ---------------------------------------------------------

    const float perpendicularX =
        -directionY;

    const float perpendicularY =
        directionX;


    // ---------------------------------------------------------
    // SURVEILLANCE CONE SIZE
    // ---------------------------------------------------------

    const float coneLength = 190.0f;
    const float coneWidth  = 65.0f;


    const float centerEndX =
        cameraX +
        directionX *
        coneLength;

    const float centerEndY =
        cameraY +
        directionY *
        coneLength;


    const float leftEndX =
        centerEndX +
        perpendicularX *
        coneWidth;

    const float leftEndY =
        centerEndY +
        perpendicularY *
        coneWidth;


    const float rightEndX =
        centerEndX -
        perpendicularX *
        coneWidth;

    const float rightEndY =
        centerEndY -
        perpendicularY *
        coneWidth;


    // ---------------------------------------------------------
    // SURVEILLANCE CONE
    // ---------------------------------------------------------

    glEnable(GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    glColor4f(
        0.80f,
        0.18f,
        0.12f,
        0.16f
    );

    glBegin(GL_TRIANGLES);

    glVertex2f(
        cameraX,
        cameraY
    );

    glVertex2f(
        leftEndX,
        leftEndY
    );

    glVertex2f(
        rightEndX,
        rightEndY
    );

    glEnd();

    glDisable(GL_BLEND);


    // ---------------------------------------------------------
    // SURVEILLANCE CONE EDGES
    // DDA
    // ---------------------------------------------------------

    glColor3f(
        0.72f,
        0.20f,
        0.15f
    );

    drawDDALine(
        cameraX,
        cameraY,
        leftEndX,
        leftEndY
    );

    drawDDALine(
        cameraX,
        cameraY,
        rightEndX,
        rightEndY
    );


    // ---------------------------------------------------------
    // CAMERA WALL BRACKET
    // ---------------------------------------------------------

    glColor3f(
        0.30f,
        0.34f,
        0.38f
    );

    drawScanlineFilledRectangle(
        cameraX - 12.0f,
        cameraY - 27.0f,
        cameraX + 12.0f,
        cameraY - 10.0f
    );


    // ---------------------------------------------------------
    // BRACKET OUTLINE
    // ---------------------------------------------------------

    glColor3f(
        0.65f,
        0.69f,
        0.73f
    );

    drawDDALine(
        cameraX - 12.0f,
        cameraY - 27.0f,
        cameraX + 12.0f,
        cameraY - 27.0f
    );

    drawDDALine(
        cameraX + 12.0f,
        cameraY - 27.0f,
        cameraX + 12.0f,
        cameraY - 10.0f
    );

    drawDDALine(
        cameraX + 12.0f,
        cameraY - 10.0f,
        cameraX - 12.0f,
        cameraY - 10.0f
    );

    drawDDALine(
        cameraX - 12.0f,
        cameraY - 10.0f,
        cameraX - 12.0f,
        cameraY - 27.0f
    );


    // ---------------------------------------------------------
    // CAMERA BODY
    // ---------------------------------------------------------

    glColor3f(
        0.42f,
        0.46f,
        0.50f
    );

    glBegin(GL_POLYGON);

    glVertex2f(
        cameraX - 25.0f,
        cameraY - 8.0f
    );

    glVertex2f(
        cameraX + 5.0f,
        cameraY - 13.0f
    );

    glVertex2f(
        cameraX + 24.0f,
        cameraY - 2.0f
    );

    glVertex2f(
        cameraX + 19.0f,
        cameraY + 15.0f
    );

    glVertex2f(
        cameraX - 15.0f,
        cameraY + 12.0f
    );

    glEnd();


    // ---------------------------------------------------------
    // CAMERA BODY OUTLINE
    // ---------------------------------------------------------

    glColor3f(
        0.76f,
        0.79f,
        0.82f
    );

    drawDDALine(
        cameraX - 25.0f,
        cameraY - 8.0f,
        cameraX + 5.0f,
        cameraY - 13.0f
    );

    drawDDALine(
        cameraX + 5.0f,
        cameraY - 13.0f,
        cameraX + 24.0f,
        cameraY - 2.0f
    );

    drawDDALine(
        cameraX + 24.0f,
        cameraY - 2.0f,
        cameraX + 19.0f,
        cameraY + 15.0f
    );

    drawDDALine(
        cameraX + 19.0f,
        cameraY + 15.0f,
        cameraX - 15.0f,
        cameraY + 12.0f
    );


    // ---------------------------------------------------------
    // CAMERA LENS
    // ---------------------------------------------------------

    const float lensX =
        cameraX + 19.0f;

    const float lensY =
        cameraY;


    // Lens outer ring
    glColor3f(
        0.20f,
        0.23f,
        0.26f
    );

    drawMidpointCircle(
        static_cast<int>(lensX),
        static_cast<int>(lensY),
        10
    );


    // Lens inner ring
    glColor3f(
        0.05f,
        0.10f,
        0.13f
    );

    drawMidpointCircle(
        static_cast<int>(lensX),
        static_cast<int>(lensY),
        6
    );


    // ---------------------------------------------------------
    // LENS ACTIVE INDICATOR
    // ---------------------------------------------------------

    glColor3f(
        0.10f,
        0.90f,
        0.68f
    );

    glBegin(GL_POINTS);

    glVertex2f(
        lensX,
        lensY
    );

    glEnd();


    // ---------------------------------------------------------
    // CAMERA SIDE DETAIL
    // ---------------------------------------------------------

    glColor3f(
        0.12f,
        0.75f,
        0.58f
    );

    drawScanlineFilledRectangle(
        cameraX - 15.0f,
        cameraY - 2.0f,
        cameraX - 7.0f,
        cameraY + 3.0f
    );


    // ---------------------------------------------------------
    // CAMERA CONNECTOR TO WALL
    // ---------------------------------------------------------

    glColor3f(
        0.36f,
        0.39f,
        0.42f
    );

    drawDDALine(
        cameraX - 18.0f,
        cameraY - 10.0f,
        cameraX - 30.0f,
        cameraY - 25.0f
    );
}

// =============================================================
// ROOM 1 — LOCKED DOOR
// =============================================================

void Renderer::drawLockedDoor(
    bool doorUnlocked,
    float doorOpenAmount
)
{
    // =========================================================
    // LOCKED DOOR
    // =========================================================
    //
    // Closed:
    //
    //       ┌─────┐
    //       │ 🔴  │
    //       │     │
    //       │     │
    //       └─────┘
    //
    // Open:
    //
    //       │     │
    //       │ 🟢  │
    //       │     │
    //
    // doorOpenAmount:
    //
    // 0.0 = closed
    // 1.0 = fully open
    // =========================================================


    const float left   = 1105.0f;
    const float right  = 1210.0f;
    const float top    = 280.0f;
    const float bottom = 460.0f;


    // ---------------------------------------------------------
    // Door dimensions
    // ---------------------------------------------------------

    const float doorWidth =
        right - left;


    // ---------------------------------------------------------
    // Maximum movement of each door panel
    // ---------------------------------------------------------

    const float panelMove =
        (doorWidth * 0.5f) *
        doorOpenAmount;


    // ---------------------------------------------------------
    // LEFT DOOR PANEL
    // ---------------------------------------------------------

    const float leftPanelLeft  = left;
    const float leftPanelRight =
        left + doorWidth * 0.5f - panelMove;


    if (leftPanelRight > leftPanelLeft)
    {
        glColor3f(
            0.08f,
            0.10f,
            0.12f
        );

        drawScanlineFilledRectangle(
            leftPanelLeft,
            top,
            leftPanelRight,
            bottom
        );


        // Panel outline

        glColor3f(
            0.25f,
            0.45f,
            0.48f
        );

        drawDDALine(
            leftPanelLeft,
            top,
            leftPanelRight,
            top
        );

        drawDDALine(
            leftPanelLeft,
            top,
            leftPanelLeft,
            bottom
        );

        drawDDALine(
            leftPanelLeft,
            bottom,
            leftPanelRight,
            bottom
        );
    }


    // ---------------------------------------------------------
    // RIGHT DOOR PANEL
    // ---------------------------------------------------------

    const float rightPanelLeft =
        left + doorWidth * 0.5f + panelMove;

    const float rightPanelRight =
        right;


    if (rightPanelRight > rightPanelLeft)
    {
        glColor3f(
            0.08f,
            0.10f,
            0.12f
        );

        drawScanlineFilledRectangle(
            rightPanelLeft,
            top,
            rightPanelRight,
            bottom
        );


        // Panel outline

        glColor3f(
            0.25f,
            0.45f,
            0.48f
        );

        drawDDALine(
            rightPanelLeft,
            top,
            rightPanelRight,
            top
        );

        drawDDALine(
            rightPanelRight,
            top,
            rightPanelRight,
            bottom
        );

        drawDDALine(
            rightPanelRight,
            bottom,
            rightPanelLeft,
            bottom
        );
    }


    // =========================================================
    // CENTER LOCK / STATUS
    // =========================================================

    //
    // During normal locked state:
    // RED
    //
    // Once access is accepted:
    // GREEN
    //

    if (!doorUnlocked)
    {
        glColor3f(
            0.80f,
            0.08f,
            0.06f
        );
    }
    else
    {
        glColor3f(
            0.10f,
            0.80f,
            0.20f
        );
    }


    // Small status panel

    const float indicatorX = 1157.0f;
    const float indicatorY = 370.0f;

    drawScanlineFilledRectangle(
        indicatorX - 10.0f,
        indicatorY - 10.0f,
        indicatorX + 10.0f,
        indicatorY + 10.0f
    );


    // ---------------------------------------------------------
    // Indicator outline
    // ---------------------------------------------------------

    glColor3f(
        0.35f,
        0.55f,
        0.55f
    );

    drawDDALine(
        indicatorX - 10.0f,
        indicatorY - 10.0f,
        indicatorX + 10.0f,
        indicatorY - 10.0f
    );

    drawDDALine(
        indicatorX + 10.0f,
        indicatorY - 10.0f,
        indicatorX + 10.0f,
        indicatorY + 10.0f
    );

    drawDDALine(
        indicatorX + 10.0f,
        indicatorY + 10.0f,
        indicatorX - 10.0f,
        indicatorY + 10.0f
    );

    drawDDALine(
        indicatorX - 10.0f,
        indicatorY + 10.0f,
        indicatorX - 10.0f,
        indicatorY - 10.0f
    );


    // =========================================================
    // DOOR FRAME
    // =========================================================

    glColor3f(
        0.18f,
        0.32f,
        0.35f
    );

    drawDDALine(
        left,
        top,
        right,
        top
    );

    drawDDALine(
        left,
        top,
        left,
        bottom
    );

    drawDDALine(
        right,
        top,
        right,
        bottom
    );

    drawDDALine(
        left,
        bottom,
        right,
        bottom
    );
}

// =============================================================
// PLAYER
// =============================================================

void Renderer::drawPlayer(
    float x,
    float y
)
{
    // ---------------------------------------------------------
    // PLAYER DIMENSIONS
    // ---------------------------------------------------------

    const int headRadius = 8;

    const float bodyTop =
        y + 9.0f;

    const float bodyBottom =
        y + 32.0f;

    const float bodyLeft =
        x - 7.0f;

    const float bodyRight =
        x + 7.0f;


    // ---------------------------------------------------------
    // PLAYER HEAD
    //
    // Midpoint Circle Algorithm
    // ---------------------------------------------------------

    glColor3f(
        0.20f,
        0.90f,
        0.35f
    );

    drawMidpointCircle(
        static_cast<int>(x),
        static_cast<int>(y),
        headRadius
    );


    // ---------------------------------------------------------
    // PLAYER BODY
    // ---------------------------------------------------------

    glColor3f(
        0.12f,
        0.75f,
        0.28f
    );

    drawScanlineFilledRectangle(
        bodyLeft,
        bodyTop,
        bodyRight,
        bodyBottom
    );


    // ---------------------------------------------------------
    // BODY OUTLINE
    //
    // DDA line drawing
    // ---------------------------------------------------------

    glColor3f(
        0.30f,
        1.00f,
        0.45f
    );

    drawDDALine(
        bodyLeft,
        bodyTop,
        bodyRight,
        bodyTop
    );

    drawDDALine(
        bodyRight,
        bodyTop,
        bodyRight,
        bodyBottom
    );

    drawDDALine(
        bodyRight,
        bodyBottom,
        bodyLeft,
        bodyBottom
    );

    drawDDALine(
        bodyLeft,
        bodyBottom,
        bodyLeft,
        bodyTop
    );


    // ---------------------------------------------------------
    // LEFT LEG
    // ---------------------------------------------------------

    drawDDALine(
        x - 3.0f,
        bodyBottom,
        x - 5.0f,
        bodyBottom + 12.0f
    );


    // ---------------------------------------------------------
    // RIGHT LEG
    // ---------------------------------------------------------

    drawDDALine(
        x + 3.0f,
        bodyBottom,
        x + 5.0f,
        bodyBottom + 12.0f
    );


    // ---------------------------------------------------------
    // LEFT ARM
    // ---------------------------------------------------------

    drawDDALine(
        bodyLeft,
        bodyTop + 5.0f,
        bodyLeft - 8.0f,
        bodyTop + 15.0f
    );


    // ---------------------------------------------------------
    // RIGHT ARM
    // ---------------------------------------------------------

    drawDDALine(
        bodyRight,
        bodyTop + 5.0f,
        bodyRight + 8.0f,
        bodyTop + 15.0f
    );
}

// =============================================================
// INTERACTION PROMPT
// =============================================================

void Renderer::drawInteractionPrompt(
    float x,
    float y
)
{
    // ---------------------------------------------------------
    // [E] BOX
    // ---------------------------------------------------------

    const float boxLeft   = x - 22.0f;
    const float boxTop    = y - 22.0f;
    const float boxRight  = x + 22.0f;
    const float boxBottom = y + 22.0f;


    // Background

    glColor3f(
        0.05f,
        0.05f,
        0.05f
    );

    drawScanlineFilledRectangle(
        boxLeft,
        boxTop,
        boxRight,
        boxBottom
    );


    // Border

    glColor3f(
        1.0f,
        1.0f,
        1.0f
    );

    drawDDALine(
        boxLeft,
        boxTop,
        boxRight,
        boxTop
    );

    drawDDALine(
        boxRight,
        boxTop,
        boxRight,
        boxBottom
    );

    drawDDALine(
        boxRight,
        boxBottom,
        boxLeft,
        boxBottom
    );

    drawDDALine(
        boxLeft,
        boxBottom,
        boxLeft,
        boxTop
    );


    // ---------------------------------------------------------
    // E LETTER
    // ---------------------------------------------------------

    const float eLeft   = x - 7.0f;
    const float eRight  = x + 7.0f;

    const float eTop    = y - 11.0f;
    const float eMiddle = y;
    const float eBottom = y + 11.0f;


    glColor3f(
        1.0f,
        1.0f,
        1.0f
    );


    drawDDALine(
        eLeft,
        eTop,
        eLeft,
        eBottom
    );

    drawDDALine(
        eLeft,
        eTop,
        eRight,
        eTop
    );

    drawDDALine(
        eLeft,
        eMiddle,
        eRight - 2.0f,
        eMiddle
    );

    drawDDALine(
        eLeft,
        eBottom,
        eRight,
        eBottom
    );
}

// =============================================================
// PUZZLE DIGIT
// =============================================================

void Renderer::drawPuzzleDigit(
    int digit,
    float x,
    float y,
    float scale
)
{
    const float width = 35.0f * scale;
    const float height = 50.0f * scale;

    bool top = false;
    bool middle = false;
    bool bottom = false;
    bool upperLeft = false;
    bool upperRight = false;
    bool lowerLeft = false;
    bool lowerRight = false;

    switch (digit)
    {
        case 0:
            top = bottom = true;
            upperLeft = upperRight = true;
            lowerLeft = lowerRight = true;
            break;

        case 1:
            upperRight = lowerRight = true;
            break;

        case 2:
            top = middle = bottom = true;
            upperRight = lowerLeft = true;
            break;

        case 3:
            top = middle = bottom = true;
            upperRight = lowerRight = true;
            break;

        case 4:
            middle = true;
            upperLeft = upperRight = true;
            lowerRight = true;
            break;

        case 5:
            top = middle = bottom = true;
            upperLeft = lowerRight = true;
            break;

        case 6:
            top = middle = bottom = true;
            upperLeft = lowerLeft = lowerRight = true;
            break;

        case 7:
            top = true;
            upperRight = lowerRight = true;
            break;

        case 8:
            top = middle = bottom = true;
            upperLeft = upperRight = true;
            lowerLeft = lowerRight = true;
            break;

        case 9:
            top = middle = bottom = true;
            upperLeft = upperRight = lowerRight = true;
            break;

        default:
            return;
    }

    if (top)
        drawDDALine(x, y, x + width, y);

    if (middle)
        drawDDALine(
            x,
            y + height / 2.0f,
            x + width,
            y + height / 2.0f
        );

    if (bottom)
        drawDDALine(
            x,
            y + height,
            x + width,
            y + height
        );

    if (upperLeft)
        drawDDALine(
            x,
            y,
            x,
            y + height / 2.0f
        );

    if (upperRight)
        drawDDALine(
            x + width,
            y,
            x + width,
            y + height / 2.0f
        );

    if (lowerLeft)
        drawDDALine(
            x,
            y + height / 2.0f,
            x,
            y + height
        );

    if (lowerRight)
        drawDDALine(
            x + width,
            y + height / 2.0f,
            x + width,
            y + height
        );
}


// =============================================================
// COMPUTER SOLVED OBJECTIVE MESSAGE
// =============================================================

void Renderer::drawComputerSolvedMessage()
{
    // ---------------------------------------------------------
    // DARKEN THE ROOM
    // ---------------------------------------------------------

    glEnable(GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    glColor4f(
        0.0f,
        0.0f,
        0.0f,
        0.72f
    );

    glBegin(GL_QUADS);

        glVertex2f(0.0f, 0.0f);
        glVertex2f(LOGICAL_WIDTH, 0.0f);
        glVertex2f(LOGICAL_WIDTH, LOGICAL_HEIGHT);
        glVertex2f(0.0f, LOGICAL_HEIGHT);

    glEnd();

    glDisable(GL_BLEND);


    // ---------------------------------------------------------
    // MESSAGE PANEL
    // ---------------------------------------------------------

    glColor3f(
        0.035f,
        0.075f,
        0.085f
    );

    drawScanlineFilledRectangle(
        300.0f,
        220.0f,
        980.0f,
        500.0f
    );


    // ---------------------------------------------------------
    // PANEL OUTLINE
    // ---------------------------------------------------------

    glColor3f(
        0.30f,
        0.62f,
        0.66f
    );

    drawDDALine(
        300.0f, 220.0f,
        980.0f, 220.0f
    );

    drawDDALine(
        980.0f, 220.0f,
        980.0f, 500.0f
    );

    drawDDALine(
        980.0f, 500.0f,
        300.0f, 500.0f
    );

    drawDDALine(
        300.0f, 500.0f,
        300.0f, 220.0f
    );


    // ---------------------------------------------------------
    // LETTER DRAWING
    // ---------------------------------------------------------

    glColor3f(
        0.72f,
        0.95f,
        0.95f
    );


    // Each character is approximately 30 x 30.

    auto drawLetter =
        [this](char letter, float x, float y)
    {
        switch (letter)
        {
            // -------------------------------------------------
            // G
            // -------------------------------------------------

            case 'G':

                drawDDALine(
                    x + 30, y,
                    x, y
                );

                drawDDALine(
                    x, y,
                    x, y + 30
                );

                drawDDALine(
                    x, y + 30,
                    x + 30, y + 30
                );

                drawDDALine(
                    x + 30, y + 30,
                    x + 30, y + 18
                );

                drawDDALine(
                    x + 30, y + 18,
                    x + 15, y + 18
                );

                break;


            // -------------------------------------------------
            // O
            // -------------------------------------------------

            case 'O':

                drawDDALine(
                    x, y,
                    x + 30, y
                );

                drawDDALine(
                    x, y,
                    x, y + 30
                );

                drawDDALine(
                    x + 30, y,
                    x + 30, y + 30
                );

                drawDDALine(
                    x, y + 30,
                    x + 30, y + 30
                );

                break;


            // -------------------------------------------------
            // T
            // -------------------------------------------------

            case 'T':

                drawDDALine(
                    x, y,
                    x + 30, y
                );

                drawDDALine(
                    x + 15, y,
                    x + 15, y + 30
                );

                break;


            // -------------------------------------------------
            // H
            // -------------------------------------------------

            case 'H':

                drawDDALine(
                    x, y,
                    x, y + 30
                );

                drawDDALine(
                    x + 30, y,
                    x + 30, y + 30
                );

                drawDDALine(
                    x, y + 15,
                    x + 30, y + 15
                );

                break;


            // -------------------------------------------------
            // E
            // -------------------------------------------------

            case 'E':

                drawDDALine(
                    x, y,
                    x, y + 30
                );

                drawDDALine(
                    x, y,
                    x + 30, y
                );

                drawDDALine(
                    x, y + 15,
                    x + 25, y + 15
                );

                drawDDALine(
                    x, y + 30,
                    x + 30, y + 30
                );

                break;


            // -------------------------------------------------
            // C
            // -------------------------------------------------

            case 'C':

                drawDDALine(
                    x + 30, y,
                    x, y
                );

                drawDDALine(
                    x, y,
                    x, y + 30
                );

                drawDDALine(
                    x, y + 30,
                    x + 30, y + 30
                );

                break;


            // -------------------------------------------------
            // A
            // -------------------------------------------------

            case 'A':

                drawDDALine(
                    x, y + 30,
                    x, y + 10
                );

                drawDDALine(
                    x, y + 10,
                    x + 15, y
                );

                drawDDALine(
                    x + 15, y,
                    x + 30, y + 10
                );

                drawDDALine(
                    x + 30, y + 10,
                    x + 30, y + 30
                );

                drawDDALine(
                    x + 5, y + 18,
                    x + 25, y + 18
                );

                break;


            // -------------------------------------------------
            // B
            // -------------------------------------------------

            case 'B':

                drawDDALine(
                    x, y,
                    x, y + 30
                );

                drawDDALine(
                    x, y,
                    x + 22, y
                );

                drawDDALine(
                    x + 22, y,
                    x + 22, y + 15
                );

                drawDDALine(
                    x + 22, y + 15,
                    x, y + 15
                );

                drawDDALine(
                    x + 22, y + 15,
                    x + 22, y + 30
                );

                drawDDALine(
                    x + 22, y + 30,
                    x, y + 30
                );

                break;


            // -------------------------------------------------
            // I
            // -------------------------------------------------

            case 'I':

                drawDDALine(
                    x, y,
                    x + 30, y
                );

                drawDDALine(
                    x + 15, y,
                    x + 15, y + 30
                );

                drawDDALine(
                    x, y + 30,
                    x + 30, y + 30
                );

                break;


            // -------------------------------------------------
            // N
            // -------------------------------------------------

            case 'N':

                drawDDALine(
                    x, y + 30,
                    x, y
                );

                drawDDALine(
                    x, y,
                    x + 30, y + 30
                );

                drawDDALine(
                    x + 30, y + 30,
                    x + 30, y
                );

                break;
        }
    };


    // =========================================================
    // "GO TO THE"
    // =========================================================

    const float y1 = 270.0f;


    // GO

    drawLetter('G', 430.0f, y1);
    drawLetter('O', 475.0f, y1);


    // TO

    drawLetter('T', 565.0f, y1);
    drawLetter('O', 610.0f, y1);


    // THE

    drawLetter('T', 700.0f, y1);
    drawLetter('H', 745.0f, y1);
    drawLetter('E', 790.0f, y1);


    // =========================================================
    // "CABINET"
    // =========================================================

    const float y2 = 365.0f;


    drawLetter('C', 420.0f, y2);
    drawLetter('A', 465.0f, y2);
    drawLetter('B', 510.0f, y2);
    drawLetter('I', 555.0f, y2);
    drawLetter('N', 600.0f, y2);
    drawLetter('E', 645.0f, y2);

    // FINAL T

    drawLetter('T', 690.0f, y2);
}

// =============================================================
// ACCESS CARD MESSAGE
// =============================================================

void Renderer::drawAccessCardMessage()
{
    // =========================================================
    // DARKEN BACKGROUND
    // =========================================================

    glEnable(GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    glColor4f(
        0.0f,
        0.0f,
        0.0f,
        0.72f
    );

    glBegin(GL_QUADS);

        glVertex2f(0.0f, 0.0f);
        glVertex2f(LOGICAL_WIDTH, 0.0f);
        glVertex2f(LOGICAL_WIDTH, LOGICAL_HEIGHT);
        glVertex2f(0.0f, LOGICAL_HEIGHT);

    glEnd();

    glDisable(GL_BLEND);


    // =========================================================
    // MESSAGE PANEL
    // =========================================================

    glColor3f(
        0.025f,
        0.055f,
        0.065f
    );

    drawScanlineFilledRectangle(
        300.0f,
        100.0f,
        980.0f,
        620.0f
    );


    // ---------------------------------------------------------
    // PANEL BORDER
    // ---------------------------------------------------------

    glColor3f(
        0.30f,
        0.70f,
        0.73f
    );

    drawDDALine(
        300.0f, 100.0f,
        980.0f, 100.0f
    );

    drawDDALine(
        980.0f, 100.0f,
        980.0f, 620.0f
    );

    drawDDALine(
        980.0f, 620.0f,
        300.0f, 620.0f
    );

    drawDDALine(
        300.0f, 620.0f,
        300.0f, 100.0f
    );


    // =========================================================
    // SMALL ACCESS CARD
    // =========================================================

    const float cardLeft   = 430.0f;
    const float cardRight  = 850.0f;
    const float cardTop    = 135.0f;
    const float cardBottom = 295.0f;


    // ---------------------------------------------------------
    // CARD BODY
    // ---------------------------------------------------------

    glColor3f(
        0.55f,
        0.88f,
        0.88f
    );

    drawScanlineFilledRectangle(
        cardLeft,
        cardTop,
        cardRight,
        cardBottom
    );


    // ---------------------------------------------------------
    // CARD HEADER
    // ---------------------------------------------------------

    glColor3f(
        0.07f,
        0.24f,
        0.28f
    );

    drawScanlineFilledRectangle(
        cardLeft,
        cardTop + 30.0f,
        cardRight,
        cardTop + 58.0f
    );


    // ---------------------------------------------------------
    // GOLD CHIP
    // ---------------------------------------------------------

    glColor3f(
        0.68f,
        0.55f,
        0.20f
    );

    drawScanlineFilledRectangle(
        cardLeft + 75.0f,
        cardTop + 75.0f,
        cardLeft + 135.0f,
        cardTop + 115.0f
    );


    // Chip lines

    glColor3f(
        0.35f,
        0.30f,
        0.12f
    );

    drawDDALine(
        cardLeft + 105.0f,
        cardTop + 75.0f,
        cardLeft + 105.0f,
        cardTop + 115.0f
    );

    drawDDALine(
        cardLeft + 75.0f,
        cardTop + 95.0f,
        cardLeft + 135.0f,
        cardTop + 95.0f
    );


    // ---------------------------------------------------------
    // CARD INFORMATION LINES
    // ---------------------------------------------------------

    glColor3f(
        0.20f,
        0.40f,
        0.43f
    );

    drawDDALine(
        cardLeft + 190.0f,
        cardTop + 80.0f,
        cardRight - 55.0f,
        cardTop + 80.0f
    );

    drawDDALine(
        cardLeft + 190.0f,
        cardTop + 108.0f,
        cardRight - 120.0f,
        cardTop + 108.0f
    );


    // =========================================================
    // GEOMETRIC LETTER FUNCTION
    // =========================================================

    glColor3f(
        0.72f,
        0.95f,
        0.95f
    );


    auto drawLetter =
        [this](char letter, float x, float y)
    {
        const float w = 30.0f;
        const float h = 44.0f;

        switch (letter)
        {
            // -------------------------------------------------
            // A
            // -------------------------------------------------

            case 'A':

                drawDDALine(
                    x,
                    y + h,
                    x + w * 0.5f,
                    y
                );

                drawDDALine(
                    x + w * 0.5f,
                    y,
                    x + w,
                    y + h
                );

                drawDDALine(
                    x + 7.0f,
                    y + 25.0f,
                    x + 23.0f,
                    y + 25.0f
                );

                break;


            // -------------------------------------------------
            // B
            // -------------------------------------------------

            case 'B':

                drawDDALine(
                    x,
                    y,
                    x,
                    y + h
                );

                drawDDALine(
                    x,
                    y,
                    x + 20.0f,
                    y
                );

                drawDDALine(
                    x + 20.0f,
                    y,
                    x + 28.0f,
                    y + 8.0f
                );

                drawDDALine(
                    x + 28.0f,
                    y + 8.0f,
                    x + 20.0f,
                    y + 22.0f
                );

                drawDDALine(
                    x + 20.0f,
                    y + 22.0f,
                    x,
                    y + 22.0f
                );

                drawDDALine(
                    x + 20.0f,
                    y + 22.0f,
                    x + 28.0f,
                    y + 31.0f
                );

                drawDDALine(
                    x + 28.0f,
                    y + 31.0f,
                    x + 20.0f,
                    y + h
                );

                drawDDALine(
                    x + 20.0f,
                    y + h,
                    x,
                    y + h
                );

                break;


            // -------------------------------------------------
            // C
            // -------------------------------------------------

            case 'C':

                drawDDALine(
                    x + w,
                    y,
                    x,
                    y
                );

                drawDDALine(
                    x,
                    y,
                    x,
                    y + h
                );

                drawDDALine(
                    x,
                    y + h,
                    x + w,
                    y + h
                );

                break;


            // -------------------------------------------------
            // D
            // -------------------------------------------------

            case 'D':

                drawDDALine(
                    x,
                    y,
                    x,
                    y + h
                );

                drawDDALine(
                    x,
                    y,
                    x + 20.0f,
                    y
                );

                drawDDALine(
                    x + 20.0f,
                    y,
                    x + w,
                    y + 10.0f
                );

                drawDDALine(
                    x + w,
                    y + 10.0f,
                    x + w,
                    y + 34.0f
                );

                drawDDALine(
                    x + w,
                    y + 34.0f,
                    x + 20.0f,
                    y + h
                );

                drawDDALine(
                    x + 20.0f,
                    y + h,
                    x,
                    y + h
                );

                break;


            // -------------------------------------------------
            // E
            // -------------------------------------------------

            case 'E':

                drawDDALine(
                    x,
                    y,
                    x,
                    y + h
                );

                drawDDALine(
                    x,
                    y,
                    x + w,
                    y
                );

                drawDDALine(
                    x,
                    y + h * 0.5f,
                    x + 23.0f,
                    y + h * 0.5f
                );

                drawDDALine(
                    x,
                    y + h,
                    x + w,
                    y + h
                );

                break;


            // -------------------------------------------------
            // I
            // -------------------------------------------------

            case 'I':

                drawDDALine(
                    x + w * 0.5f,
                    y,
                    x + w * 0.5f,
                    y + h
                );

                break;


            // -------------------------------------------------
            // N
            // -------------------------------------------------

            case 'N':

                drawDDALine(
                    x,
                    y + h,
                    x,
                    y
                );

                drawDDALine(
                    x,
                    y,
                    x + w,
                    y + h
                );

                drawDDALine(
                    x + w,
                    y + h,
                    x + w,
                    y
                );

                break;


            // -------------------------------------------------
            // O
            // -------------------------------------------------

            case 'O':

                drawDDALine(
                    x,
                    y,
                    x + w,
                    y
                );

                drawDDALine(
                    x + w,
                    y,
                    x + w,
                    y + h
                );

                drawDDALine(
                    x + w,
                    y + h,
                    x,
                    y + h
                );

                drawDDALine(
                    x,
                    y + h,
                    x,
                    y
                );

                break;


            // -------------------------------------------------
            // R
            // -------------------------------------------------

            case 'R':

                drawDDALine(
                    x,
                    y,
                    x,
                    y + h
                );

                drawDDALine(
                    x,
                    y,
                    x + 20.0f,
                    y
                );

                drawDDALine(
                    x + 20.0f,
                    y,
                    x + w,
                    y + 10.0f
                );

                drawDDALine(
                    x + w,
                    y + 10.0f,
                    x + 20.0f,
                    y + 22.0f
                );

                drawDDALine(
                    x + 20.0f,
                    y + 22.0f,
                    x,
                    y + 22.0f
                );

                drawDDALine(
                    x + 15.0f,
                    y + 22.0f,
                    x + w,
                    y + h
                );

                break;


            // -------------------------------------------------
            // S
            // -------------------------------------------------

            case 'S':

                // TOP
                drawDDALine(
                    x + w,
                    y,
                    x,
                    y
                );

                // LEFT UPPER
                drawDDALine(
                    x,
                    y,
                    x,
                    y + h * 0.5f
                );

                // MIDDLE
                drawDDALine(
                    x,
                    y + h * 0.5f,
                    x + w,
                    y + h * 0.5f
                );

                // RIGHT LOWER
                drawDDALine(
                    x + w,
                    y + h * 0.5f,
                    x + w,
                    y + h
                );

                // BOTTOM
                drawDDALine(
                    x + w,
                    y + h,
                    x,
                    y + h
                );

                break;


            // -------------------------------------------------
            // T
            // -------------------------------------------------

            case 'T':

                drawDDALine(
                    x,
                    y,
                    x + w,
                    y
                );

                drawDDALine(
                    x + w * 0.5f,
                    y,
                    x + w * 0.5f,
                    y + h
                );

                break;
        }
    };


    // =========================================================
    // ACCESS CARD
    // =========================================================

    //
    // ACCESS CARD
    //
    //  A C C E S S     C A R D
    //

    const float letterSpacing = 38.0f;

    const float accessStartX = 370.0f;
    const float accessY      = 365.0f;


    drawLetter('A', accessStartX + letterSpacing * 0, accessY);
    drawLetter('C', accessStartX + letterSpacing * 1, accessY);
    drawLetter('C', accessStartX + letterSpacing * 2, accessY);
    drawLetter('E', accessStartX + letterSpacing * 3, accessY);
    drawLetter('S', accessStartX + letterSpacing * 4, accessY);
    drawLetter('S', accessStartX + letterSpacing * 5, accessY);


    // ---------------------------------------------------------
    // CARD
    // ---------------------------------------------------------

    const float cardWordX = 650.0f;

    drawLetter('C', cardWordX + letterSpacing * 0, accessY);
    drawLetter('A', cardWordX + letterSpacing * 1, accessY);
    drawLetter('R', cardWordX + letterSpacing * 2, accessY);
    drawLetter('D', cardWordX + letterSpacing * 3, accessY);


    // =========================================================
    // OBTAINED
    // =========================================================

    //
    //        O B T A I N E D
    //

    const float obtainedStartX = 475.0f;
    const float obtainedY      = 465.0f;


    drawLetter(
        'O',
        obtainedStartX + letterSpacing * 0,
        obtainedY
    );

    drawLetter(
        'B',
        obtainedStartX + letterSpacing * 1,
        obtainedY
    );

    drawLetter(
        'T',
        obtainedStartX + letterSpacing * 2,
        obtainedY
    );

    drawLetter(
        'A',
        obtainedStartX + letterSpacing * 3,
        obtainedY
    );

    drawLetter(
        'I',
        obtainedStartX + letterSpacing * 4,
        obtainedY
    );

    drawLetter(
        'N',
        obtainedStartX + letterSpacing * 5,
        obtainedY
    );

    drawLetter(
        'E',
        obtainedStartX + letterSpacing * 6,
        obtainedY
    );

    drawLetter(
        'D',
        obtainedStartX + letterSpacing * 7,
        obtainedY
    );
}


void Renderer::drawAccessGrantedMessage()
{
    // =========================================================
    // DARKEN ROOM
    // =========================================================

    glEnable(GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    glColor4f(
        0.0f,
        0.0f,
        0.0f,
        0.72f
    );

    glBegin(GL_QUADS);

        glVertex2f(0.0f, 0.0f);
        glVertex2f(LOGICAL_WIDTH, 0.0f);
        glVertex2f(LOGICAL_WIDTH, LOGICAL_HEIGHT);
        glVertex2f(0.0f, LOGICAL_HEIGHT);

    glEnd();

    glDisable(GL_BLEND);


    // =========================================================
    // MESSAGE PANEL
    // =========================================================

    glColor3f(
        0.025f,
        0.055f,
        0.065f
    );

    drawScanlineFilledRectangle(
        300.0f,
        250.0f,
        980.0f,
        470.0f
    );


    // =========================================================
    // PANEL OUTLINE
    // =========================================================

    glColor3f(
        0.30f,
        0.70f,
        0.73f
    );

    drawDDALine(
        300.0f, 250.0f,
        980.0f, 250.0f
    );

    drawDDALine(
        980.0f, 250.0f,
        980.0f, 470.0f
    );

    drawDDALine(
        980.0f, 470.0f,
        300.0f, 470.0f
    );

    drawDDALine(
        300.0f, 470.0f,
        300.0f, 250.0f
    );


    // =========================================================
    // TEXT COLOR
    // =========================================================

    glColor3f(
        0.72f,
        0.95f,
        0.95f
    );


    // =========================================================
    // LETTER DRAWING
    // =========================================================

    auto drawLetter =
        [this](char letter, float x, float y)
    {
        const float w = 26.0f;
        const float h = 40.0f;

        switch (letter)
        {
            // =================================================
            // A
            // =================================================

            case 'A':

                drawDDALine(
                    x,
                    y + h,
                    x + w * 0.5f,
                    y
                );

                drawDDALine(
                    x + w * 0.5f,
                    y,
                    x + w,
                    y + h
                );

                drawDDALine(
                    x + 6.0f,
                    y + 23.0f,
                    x + 20.0f,
                    y + 23.0f
                );

                break;


            // =================================================
            // C
            // =================================================

            case 'C':

                drawDDALine(
                    x + w,
                    y,
                    x,
                    y
                );

                drawDDALine(
                    x,
                    y,
                    x,
                    y + h
                );

                drawDDALine(
                    x,
                    y + h,
                    x + w,
                    y + h
                );

                break;


            // =================================================
            // E
            // =================================================

            case 'E':

                drawDDALine(
                    x,
                    y,
                    x,
                    y + h
                );

                drawDDALine(
                    x,
                    y,
                    x + w,
                    y
                );

                drawDDALine(
                    x,
                    y + h * 0.5f,
                    x + 21.0f,
                    y + h * 0.5f
                );

                drawDDALine(
                    x,
                    y + h,
                    x + w,
                    y + h
                );

                break;


            // =================================================
            // S
            // =================================================

            case 'S':

                // TOP
                drawDDALine(
                    x + w,
                    y,
                    x,
                    y
                );

                // LEFT
                drawDDALine(
                    x,
                    y,
                    x,
                    y + h * 0.5f
                );

                // MIDDLE
                drawDDALine(
                    x,
                    y + h * 0.5f,
                    x + w,
                    y + h * 0.5f
                );

                // RIGHT
                drawDDALine(
                    x + w,
                    y + h * 0.5f,
                    x + w,
                    y + h
                );

                // BOTTOM
                drawDDALine(
                    x + w,
                    y + h,
                    x,
                    y + h
                );

                break;


            // =================================================
            // G
            // =================================================

            case 'G':

                drawDDALine(
                    x + w,
                    y,
                    x,
                    y
                );

                drawDDALine(
                    x,
                    y,
                    x,
                    y + h
                );

                drawDDALine(
                    x,
                    y + h,
                    x + w,
                    y + h
                );

                drawDDALine(
                    x + w,
                    y + h,
                    x + w,
                    y + 22.0f
                );

                drawDDALine(
                    x + w,
                    y + 22.0f,
                    x + 13.0f,
                    y + 22.0f
                );

                break;


            // =================================================
            // R
            // =================================================

            case 'R':

                drawDDALine(
                    x,
                    y,
                    x,
                    y + h
                );

                drawDDALine(
                    x,
                    y,
                    x + 17.0f,
                    y
                );

                drawDDALine(
                    x + 17.0f,
                    y,
                    x + w,
                    y + 9.0f
                );

                drawDDALine(
                    x + w,
                    y + 9.0f,
                    x + 17.0f,
                    y + 20.0f
                );

                drawDDALine(
                    x + 17.0f,
                    y + 20.0f,
                    x,
                    y + 20.0f
                );

                drawDDALine(
                    x + 13.0f,
                    y + 20.0f,
                    x + w,
                    y + h
                );

                break;


            // =================================================
            // N
            // =================================================

            case 'N':

                drawDDALine(
                    x,
                    y + h,
                    x,
                    y
                );

                drawDDALine(
                    x,
                    y,
                    x + w,
                    y + h
                );

                drawDDALine(
                    x + w,
                    y + h,
                    x + w,
                    y
                );

                break;


            // =================================================
            // T
            // =================================================

            case 'T':

                drawDDALine(
                    x,
                    y,
                    x + w,
                    y
                );

                drawDDALine(
                    x + w * 0.5f,
                    y,
                    x + w * 0.5f,
                    y + h
                );

                break;


            // =================================================
            // D
            // =================================================

            case 'D':

                drawDDALine(
                    x,
                    y,
                    x,
                    y + h
                );

                drawDDALine(
                    x,
                    y,
                    x + 17.0f,
                    y
                );

                drawDDALine(
                    x + 17.0f,
                    y,
                    x + w,
                    y + 9.0f
                );

                drawDDALine(
                    x + w,
                    y + 9.0f,
                    x + w,
                    y + 31.0f
                );

                drawDDALine(
                    x + w,
                    y + 31.0f,
                    x + 17.0f,
                    y + h
                );

                drawDDALine(
                    x + 17.0f,
                    y + h,
                    x,
                    y + h
                );

                break;
        }
    };


    // =========================================================
    // "ACCESS"
    // =========================================================

    // Smaller spacing so the entire phrase fits cleanly.

    const float spacing = 34.0f;

    const float accessX = 350.0f;
    const float textY   = 320.0f;


    drawLetter(
        'A',
        accessX + spacing * 0,
        textY
    );

    drawLetter(
        'C',
        accessX + spacing * 1,
        textY
    );

    drawLetter(
        'C',
        accessX + spacing * 2,
        textY
    );

    drawLetter(
        'E',
        accessX + spacing * 3,
        textY
    );

    drawLetter(
        'S',
        accessX + spacing * 4,
        textY
    );

    drawLetter(
        'S',
        accessX + spacing * 5,
        textY
    );


    // =========================================================
    // "GRANTED"
    // =========================================================

    const float grantedX = 585.0f;


    drawLetter(
        'G',
        grantedX + spacing * 0,
        textY
    );

    drawLetter(
        'R',
        grantedX + spacing * 1,
        textY
    );

    drawLetter(
        'A',
        grantedX + spacing * 2,
        textY
    );

    drawLetter(
        'N',
        grantedX + spacing * 3,
        textY
    );

    drawLetter(
        'T',
        grantedX + spacing * 4,
        textY
    );

    drawLetter(
        'E',
        grantedX + spacing * 5,
        textY
    );

    drawLetter(
        'D',
        grantedX + spacing * 6,
        textY
    );
}


// =============================================================
// PC PUZZLE POPUP
// =============================================================

void Renderer::drawComputerPuzzle(
    int enteredValue,
    int digitsEntered,
    bool error
)
{
    // ---------------------------------------------------------
    // DARK OVERLAY
    // ---------------------------------------------------------

    glEnable(GL_BLEND);

    glBlendFunc(
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_ALPHA
    );

    glColor4f(
        0.0f,
        0.0f,
        0.0f,
        0.72f
    );

    glBegin(GL_QUADS);

        glVertex2f(0.0f, 0.0f);
        glVertex2f(LOGICAL_WIDTH, 0.0f);
        glVertex2f(LOGICAL_WIDTH, LOGICAL_HEIGHT);
        glVertex2f(0.0f, LOGICAL_HEIGHT);

    glEnd();

    glDisable(GL_BLEND);


    // ---------------------------------------------------------
    // MAIN POPUP
    // ---------------------------------------------------------

    const float left = 220.0f;
    const float top = 70.0f;
    const float right = 1060.0f;
    const float bottom = 650.0f;


    glColor3f(
        0.025f,
        0.045f,
        0.055f
    );

    drawScanlineFilledRectangle(
        left,
        top,
        right,
        bottom
    );


    // ---------------------------------------------------------
    // OUTER BORDER
    // ---------------------------------------------------------

    glColor3f(
        0.55f,
        0.80f,
        0.84f
    );

    drawDDALine(left, top, right, top);
    drawDDALine(right, top, right, bottom);
    drawDDALine(right, bottom, left, bottom);
    drawDDALine(left, bottom, left, top);


    // ---------------------------------------------------------
    // INNER BORDER
    // ---------------------------------------------------------

    glColor3f(
        0.20f,
        0.40f,
        0.44f
    );

    drawDDALine(
        left + 15.0f,
        top + 15.0f,
        right - 15.0f,
        top + 15.0f
    );

    drawDDALine(
        right - 15.0f,
        top + 15.0f,
        right - 15.0f,
        bottom - 15.0f
    );

    drawDDALine(
        right - 15.0f,
        bottom - 15.0f,
        left + 15.0f,
        bottom - 15.0f
    );

    drawDDALine(
        left + 15.0f,
        bottom - 15.0f,
        left + 15.0f,
        top + 15.0f
    );


    // ---------------------------------------------------------
    // HEADER
    // ---------------------------------------------------------

    glColor3f(
        0.07f,
        0.18f,
        0.20f
    );

    drawScanlineFilledRectangle(
        245.0f,
        95.0f,
        1035.0f,
        155.0f
    );


    glColor3f(
        0.35f,
        0.78f,
        0.82f
    );

    drawDDALine(
        245.0f,
        155.0f,
        1035.0f,
        155.0f
    );


    // Status lights

    glColor3f(
        0.15f,
        0.90f,
        0.62f
    );

    drawMidpointCircle(
        275,
        125,
        7
    );


    glColor3f(
        0.85f,
        0.15f,
        0.12f
    );

    drawMidpointCircle(
        1005,
        125,
        7
    );


    // ---------------------------------------------------------
    // HEADER TITLE
    // ---------------------------------------------------------
    //
    // "SECURITY" drawn with simple DDA strokes.
    // ---------------------------------------------------------

    glColor3f(
        0.70f,
        0.95f,
        0.95f
    );


    // S
    drawDDALine(400,110,435,110);
    drawDDALine(400,110,400,125);
    drawDDALine(400,125,435,125);
    drawDDALine(435,125,435,140);
    drawDDALine(400,140,435,140);

    // E
    drawDDALine(450,110,450,140);
    drawDDALine(450,110,485,110);
    drawDDALine(450,125,480,125);
    drawDDALine(450,140,485,140);

    // C
    drawDDALine(535,110,500,110);
    drawDDALine(500,110,500,140);
    drawDDALine(500,140,535,140);

    // U
    drawDDALine(550,110,550,140);
    drawDDALine(550,140,585,140);
    drawDDALine(585,140,585,110);

    // R
    drawDDALine(600,140,600,110);
    drawDDALine(600,110,635,110);
    drawDDALine(635,110,635,125);
    drawDDALine(600,125,635,125);
    drawDDALine(620,125,640,140);

    // I
    drawDDALine(655,110,685,110);
    drawDDALine(670,110,670,140);
    drawDDALine(655,140,685,140);

    // T
    drawDDALine(700,110,735,110);
    drawDDALine(717,110,717,140);

    // Y
    drawDDALine(750,110,767,125);
    drawDDALine(784,110,767,125);
    drawDDALine(767,125,767,140);


    // ---------------------------------------------------------
    // STATUS STRIP
    // ---------------------------------------------------------

    glColor3f(
        0.16f,
        0.42f,
        0.45f
    );

    drawScanlineFilledRectangle(
        300.0f,
        175.0f,
        980.0f,
        190.0f
    );


    glColor3f(
        0.45f,
        0.82f,
        0.84f
    );

    drawDDALine(
        320.0f,
        183.0f,
        500.0f,
        183.0f
    );


    // ---------------------------------------------------------
    // PUZZLE BOX
    // ---------------------------------------------------------

    glColor3f(
        0.045f,
        0.095f,
        0.11f
    );

    drawScanlineFilledRectangle(
        300.0f,
        215.0f,
        980.0f,
        335.0f
    );


    glColor3f(
        0.30f,
        0.62f,
        0.66f
    );

    drawDDALine(300,215,980,215);
    drawDDALine(980,215,980,335);
    drawDDALine(980,335,300,335);
    drawDDALine(300,335,300,215);


    // ---------------------------------------------------------
    // SEQUENCE: 2  6  12  20  30  ?
    // ---------------------------------------------------------

    glColor3f(
        0.72f,
        0.95f,
        0.95f
    );

    drawPuzzleDigit(2, 345, 250, 0.70f);
    drawPuzzleDigit(6, 425, 250, 0.70f);

    drawPuzzleDigit(1, 505, 250, 0.70f);
    drawPuzzleDigit(2, 535, 250, 0.70f);

    drawPuzzleDigit(2, 615, 250, 0.70f);
    drawPuzzleDigit(0, 645, 250, 0.70f);

    drawPuzzleDigit(3, 725, 250, 0.70f);
    drawPuzzleDigit(0, 755, 250, 0.70f);


    // Question mark

    drawDDALine(850,250,890,250);
    drawDDALine(890,250,890,275);
    drawDDALine(890,275,850,275);
    drawDDALine(850,275,850,285);

    drawMidpointCircle(
        870,
        305,
        3
    );


    // ---------------------------------------------------------
    // ANSWER BOX
    // ---------------------------------------------------------

    glColor3f(
        0.035f,
        0.075f,
        0.09f
    );

    drawScanlineFilledRectangle(
        485.0f,
        365.0f,
        795.0f,
        450.0f
    );


    glColor3f(
        0.30f,
        0.62f,
        0.66f
    );

    drawDDALine(485,365,795,365);
    drawDDALine(795,365,795,450);
    drawDDALine(795,450,485,450);
    drawDDALine(485,450,485,365);


    // ---------------------------------------------------------
    // ENTERED ANSWER
    // ---------------------------------------------------------

    glColor3f(
        0.72f,
        0.95f,
        0.95f
    );


    if (digitsEntered == 0)
    {
        drawDDALine(
            610,
            408,
            670,
            408
        );
    }
    else if (digitsEntered == 1)
    {
        drawPuzzleDigit(
            enteredValue,
            620,
            390,
            0.85f
        );
    }
    else
    {
        drawPuzzleDigit(
            enteredValue / 10,
            565,
            390,
            0.85f
        );

        drawPuzzleDigit(
            enteredValue % 10,
            660,
            390,
            0.85f
        );
    }


    // ---------------------------------------------------------
    // ERROR INDICATOR
    // ---------------------------------------------------------

    if (error)
    {
        glColor3f(
            0.95f,
            0.12f,
            0.10f
        );

        drawDDALine(
            450,
            475,
            830,
            475
        );

        drawDDALine(
            450,
            480,
            830,
            480
        );
    }


    // ---------------------------------------------------------
    // ENTER BUTTON
    // ---------------------------------------------------------

    glColor3f(
        0.08f,
        0.25f,
        0.28f
    );

    drawScanlineFilledRectangle(
        515.0f,
        505.0f,
        765.0f,
        555.0f
    );


    glColor3f(
        0.55f,
        0.84f,
        0.86f
    );

    drawDDALine(515,505,765,505);
    drawDDALine(765,505,765,555);
    drawDDALine(765,555,515,555);
    drawDDALine(515,555,515,505);


    // Enter symbol

    drawDDALine(
        600,
        530,
        680,
        530
    );

    drawDDALine(
        600,
        530,
        615,
        518
    );

    drawDDALine(
        600,
        530,
        615,
        542
    );


    // ---------------------------------------------------------
    // BOTTOM DECORATION
    // ---------------------------------------------------------

    glColor3f(
        0.30f,
        0.58f,
        0.61f
    );

    drawDDALine(
        360,
        595,
        920,
        595
    );

    drawDDALine(
        390,
        610,
        450,
        610
    );

    drawDDALine(
        390,
        610,
        402,
        602
    );

    drawDDALine(
        390,
        610,
        402,
        618
    );
}


void Renderer::drawRoom(
    bool doorUnlocked,
    float doorOpenAmount
)
{
    

    // ---------------------------------------------------------
    // FLOOR
    // ---------------------------------------------------------

    glColor3f(
        0.055f,
        0.065f,
        0.075f
    );

    drawScanlineFilledRectangle(
        ROOM_LEFT + 1.0f,
        ROOM_TOP + 1.0f,
        ROOM_RIGHT - 1.0f,
        ROOM_BOTTOM - 1.0f
    );


    // ---------------------------------------------------------
    // ROOM WALLS
    // ---------------------------------------------------------

    glColor3f(
        0.65f,
        0.68f,
        0.72f
    );


    drawDDALine(
        ROOM_LEFT,
        ROOM_TOP,
        ROOM_RIGHT,
        ROOM_TOP
    );


    drawDDALine(
        ROOM_LEFT,
        ROOM_TOP,
        ROOM_LEFT,
        ROOM_BOTTOM
    );


    drawDDALine(
        ROOM_RIGHT,
        ROOM_TOP,
        ROOM_RIGHT,
        ROOM_BOTTOM
    );


    drawDDALine(
        ROOM_LEFT,
        ROOM_BOTTOM,
        ENTRY_LEFT,
        ROOM_BOTTOM
    );


    drawDDALine(
        ENTRY_RIGHT,
        ROOM_BOTTOM,
        ROOM_RIGHT,
        ROOM_BOTTOM
    );


    // ---------------------------------------------------------
    // ROOM 1 OBJECTS
    // ---------------------------------------------------------

    drawReceptionDesk();

    drawComputer();

    drawChair();

    drawNoticeBoard();

    drawCabinet();

    drawCrate();

    drawTable();

    // drawPlant();

    drawCCTV();

    drawLockedDoor(
    doorUnlocked,
    doorOpenAmount
);
}

// =============================================================
// SIMPLE 5x7 BITMAP TEXT
// =============================================================

void Renderer::drawChar5x7(
    char character,
    float x,
    float y,
    float scale
)
{
    static const unsigned char font[][7] =
    {
        // A-Z
        {14,17,17,31,17,17,17}, {30,17,17,30,17,17,30},
        {14,17,16,16,16,17,14}, {30,17,17,17,17,17,30},
        {31,16,16,30,16,16,31}, {31,16,16,30,16,16,16},
        {14,17,16,23,17,17,15}, {17,17,17,31,17,17,17},
        {31,4,4,4,4,4,31}, {7,2,2,2,18,18,12},
        {17,18,20,24,20,18,17}, {16,16,16,16,16,16,31},
        {17,27,21,21,17,17,17}, {17,25,21,19,17,17,17},
        {14,17,17,17,17,17,14}, {30,17,17,30,16,16,16},
        {14,17,17,17,21,18,13}, {30,17,17,30,20,18,17},
        {15,16,16,14,1,1,30}, {31,4,4,4,4,4,4},
        {17,17,17,17,17,17,14}, {17,17,17,17,17,10,4},
        {17,17,17,21,21,27,17}, {17,17,10,4,10,17,17},
        {17,17,10,4,4,4,4}, {31,1,2,4,8,16,31},
        // 0-9
        {14,17,19,21,25,17,14}, {4,12,4,4,4,4,14},
        {14,17,1,2,4,8,31}, {30,1,1,14,1,1,30},
        {2,6,10,18,31,2,2}, {31,16,16,30,1,1,30},
        {14,16,16,30,17,17,14}, {31,1,2,4,8,8,8},
        {14,17,17,14,17,17,14}, {14,17,17,15,1,1,14}
    };

    int index = -1;
    if (character >= 'A' && character <= 'Z')
        index = character - 'A';
    else if (character >= '0' && character <= '9')
        index = 26 + character - '0';

    if (index < 0)
        return;

    glBegin(GL_QUADS);
    for (int row = 0; row < 7; ++row)
    {
        for (int col = 0; col < 5; ++col)
        {
            if (font[index][row] & (1 << (4 - col)))
            {
                glVertex2f(x + col * scale, y + row * scale);
                glVertex2f(x + (col + 1) * scale, y + row * scale);
                glVertex2f(x + (col + 1) * scale, y + (row + 1) * scale);
                glVertex2f(x + col * scale, y + (row + 1) * scale);
            }
        }
    }
    glEnd();
}

void Renderer::drawText5x7(
    const char* text,
    float x,
    float y,
    float scale
)
{
    float cursorX = x;
    float cursorY = y;
    const float advance = 6.0f * scale;

    for (const char* p = text; *p != '\0'; ++p)
    {
        if (*p == '\n')
        {
            cursorX = x;
            cursorY += 9.0f * scale;
            continue;
        }

        if (*p == ' ')
        {
            cursorX += advance;
            continue;
        }

        if (*p == '.')
        {
            glBegin(GL_QUADS);

            glVertex2f(cursorX + 2 * scale, cursorY + 6 * scale);
            glVertex2f(cursorX + 3 * scale, cursorY + 6 * scale);
            glVertex2f(cursorX + 3 * scale, cursorY + 7 * scale);
            glVertex2f(cursorX + 2 * scale, cursorY + 7 * scale);

            glEnd();

            cursorX += advance;
            continue;
        }

        if (*p == ',')
        {
            glBegin(GL_QUADS);

            glVertex2f(cursorX + 2 * scale, cursorY + 5 * scale);
            glVertex2f(cursorX + 3 * scale, cursorY + 5 * scale);
            glVertex2f(cursorX + 3 * scale, cursorY + 6 * scale);
            glVertex2f(cursorX + 2 * scale, cursorY + 6 * scale);

            glVertex2f(cursorX + 2 * scale, cursorY + 6 * scale);
            glVertex2f(cursorX + 3 * scale, cursorY + 6 * scale);
            glVertex2f(cursorX + 2 * scale, cursorY + 7 * scale);
            glVertex2f(cursorX + 1 * scale, cursorY + 7 * scale);

            glEnd();

            cursorX += advance;
            continue;
        }

        if (*p == '\'')
        {
            glBegin(GL_QUADS);

            glVertex2f(cursorX + 2 * scale, cursorY + 1 * scale);
            glVertex2f(cursorX + 3 * scale, cursorY + 1 * scale);
            glVertex2f(cursorX + 3 * scale, cursorY + 3 * scale);
            glVertex2f(cursorX + 2 * scale, cursorY + 3 * scale);

            glEnd();

            cursorX += advance;
            continue;
        }

        if (*p == '?')
        {
            glBegin(GL_QUADS);
            glVertex2f(cursorX + 1*scale, cursorY + 1*scale);
            glVertex2f(cursorX + 4*scale, cursorY + 1*scale);
            glVertex2f(cursorX + 4*scale, cursorY + 2*scale);
            glVertex2f(cursorX + 1*scale, cursorY + 2*scale);
            glVertex2f(cursorX + 3*scale, cursorY + 2*scale);
            glVertex2f(cursorX + 4*scale, cursorY + 2*scale);
            glVertex2f(cursorX + 4*scale, cursorY + 4*scale);
            glVertex2f(cursorX + 3*scale, cursorY + 4*scale);
            glVertex2f(cursorX + 3*scale, cursorY + 6*scale);
            glVertex2f(cursorX + 4*scale, cursorY + 6*scale);
            glVertex2f(cursorX + 4*scale, cursorY + 7*scale);
            glVertex2f(cursorX + 3*scale, cursorY + 7*scale);
            glVertex2f(cursorX + 3*scale, cursorY + 6*scale);
            glVertex2f(cursorX + 3*scale, cursorY + 4*scale);
            glVertex2f(cursorX + 4*scale, cursorY + 4*scale);
            glEnd();
            cursorX += advance;
            continue;
        }

        if (*p == '-')
        {
            glBegin(GL_QUADS);
            glVertex2f(cursorX + 1*scale, cursorY + 3*scale);
            glVertex2f(cursorX + 5*scale, cursorY + 3*scale);
            glVertex2f(cursorX + 5*scale, cursorY + 4*scale);
            glVertex2f(cursorX + 1*scale, cursorY + 4*scale);
            glEnd();
            cursorX += advance;
            continue;
        }

        if (*p == ':')
        {
            glBegin(GL_QUADS);
            glVertex2f(cursorX + 2*scale, cursorY + 1*scale);
            glVertex2f(cursorX + 3*scale, cursorY + 1*scale);
            glVertex2f(cursorX + 3*scale, cursorY + 2*scale);
            glVertex2f(cursorX + 2*scale, cursorY + 2*scale);
            glVertex2f(cursorX + 2*scale, cursorY + 5*scale);
            glVertex2f(cursorX + 3*scale, cursorY + 5*scale);
            glVertex2f(cursorX + 3*scale, cursorY + 6*scale);
            glVertex2f(cursorX + 2*scale, cursorY + 6*scale);
            glEnd();
            cursorX += advance;
            continue;
        }

        drawChar5x7(*p, cursorX, cursorY, scale);
        cursorX += advance;
    }
}

// =============================================================
// ROOM 2 MESSAGE PANELS
// =============================================================

void Renderer::drawRoom2Message(int messageType)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.72f);
    glBegin(GL_QUADS);
    glVertex2f(0, 0); glVertex2f(LOGICAL_WIDTH, 0);
    glVertex2f(LOGICAL_WIDTH, LOGICAL_HEIGHT); glVertex2f(0, LOGICAL_HEIGHT);
    glEnd();
    glDisable(GL_BLEND);

    glColor3f(0.035f, 0.06f, 0.07f);
    drawScanlineFilledRectangle(210, 205, 1070, 515);

    glColor3f(0.30f, 0.62f, 0.66f);
    drawDDALine(210,205,1070,205); drawDDALine(1070,205,1070,515);
    drawDDALine(1070,515,210,515); drawDDALine(210,515,210,205);

    const char* title = "ELECTRICAL ROOM";
    const char* body1 = "";
    const char* body2 = "";
    const char* body3 = "";

    switch (messageType)
    {
        case 1:
            title = "ELECTRICAL ROOM";
            body1 = "EMERGENCY SYSTEMS ACTIVE";
            body2 = "MAIN POWER INACTIVE";
            body3 = "RESTORE POWER TO ESCAPE";
            break;
        case 2:
            title = "FUSE BOX";
            body1 = "THE SPARE FUSE IS IN PLACE";
            body2 = "THE GENERATOR CAN RECEIVE POWER";
            body3 = "CHECK THE WORKBENCH";
            break;
        case 3:
            title = "CABLE SYSTEM";
            body1 = "CABLES CONNECTED";
            body2 = "POWER CAN NOW REACH THE GENERATOR";
            body3 = "START THE GENERATOR";
            break;
        case 4:
            title = "GENERATOR";
            body1 = "GENERATOR STARTED";
            body2 = "EMERGENCY POWER IS NOW AVAILABLE";
            body3 = "CHECK THE ELECTRICAL PANEL";
            break;
        case 5:
            title = "ELECTRICAL PANEL";
            body1 = "EMERGENCY POWER DETECTED";
            body2 = "POWER CIRCUITS ARE ONLINE";
            body3 = "USE THE MAIN SWITCH";
            break;
        case 6:
            title = "MAIN SWITCH";
            body1 = "MAIN SWITCH ACTIVATED";
            body2 = "BUILDING POWER HAS BEEN RESTORED";
            body3 = "THE EXIT DOOR IS NOW UNLOCKED";
            break;
        case 7:
            title = "EXIT DOOR";
            body1 = "THE DOOR IS STILL LOCKED";
            body2 = "EMERGENCY POWER RESTORATION";
            body3 = "IS NOT COMPLETE";
            break;
        case 8:
            title = "EXIT DOOR";
            body1 = "ESCAPE ROUTE UNLOCKED";
            body2 = "THE ELECTRICAL SYSTEM IS RESTORED";
            body3 = "ROOM 2 COMPLETE";
            break;
        case 9:
            title = "WORKBENCH";
            body1 = "CABLE CONFIGURATION ACCEPTED";
            body2 = "THE CORRECT ORDER IS CONFIRMED";
            body3 = "GO TO THE CABLE SYSTEM";
            break;
        case 10:
            title = "SYSTEM LOCKED";
            body1 = "INSERT THE SPARE FUSE FIRST";
            body2 = "THEN RETURN TO THIS STATION";
            body3 = "PRESS E TO CONTINUE";
            break;
        case 11:
            title = "CABLE SYSTEM";
            body1 = "THE CABLE ORDER IS UNKNOWN";
            body2 = "SOLVE THE WORKBENCH PUZZLE FIRST";
            body3 = "THEN CONNECT THE CABLES";
            break;
        case 12:
            title = "GENERATOR";
            body1 = "NO POWER IS REACHING THE STARTER";
            body2 = "CONNECT THE CABLES FIRST";
            body3 = "THEN RETURN HERE";
            break;
        case 13:
            title = "ELECTRICAL PANEL";
            body1 = "SYSTEM OFFLINE";
            body2 = "THE GENERATOR MUST BE RUNNING";
            body3 = "RESTORE GENERATOR POWER FIRST";
            break;
        case 14:
            title = "MAIN SWITCH";
            body1 = "THE MAIN SWITCH CANNOT BE USED YET";
            body2 = "ACTIVATE THE ELECTRICAL PANEL FIRST";
            body3 = "THEN RETURN HERE";
            break;
        default:
            break;
    }

    glColor3f(0.75f, 0.90f, 0.92f);
    drawText5x7(title, 270, 250, 4.0f);

    glColor3f(0.68f, 0.78f, 0.80f);
    drawText5x7(body1, 270, 315, 3.0f);
    drawText5x7(body2, 270, 355, 3.0f);
    drawText5x7(body3, 270, 395, 3.0f);

    glColor3f(0.35f, 0.65f, 0.68f);
    drawText5x7("PRESS E TO CONTINUE", 270, 465, 2.5f);
}

// =============================================================
// ROOM 2 WORKBENCH PUZZLE
// =============================================================

void Renderer::drawRoom2Puzzle(
    const char* enteredSequence,
    int enteredCount,
    bool error
)
{
    // =========================================================
    // WORKBENCH CABLE PUZZLE
    // =========================================================
    // The player must interpret the clues and enter the colour
    // letters R, B, Y, G in the correct order.
    //
    // IMPORTANT:
    // The four boxes are intentionally EMPTY before input.
    // They do not display the answer or the terminal letters.
    // Each letter typed by the player appears in the next box.
    // =========================================================

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor4f(0.015f, 0.025f, 0.030f, 0.97f);

    drawScanlineFilledRectangle(
        250.0f,
        130.0f,
        1030.0f,
        590.0f
    );

    glColor4f(0.88f, 0.62f, 0.12f, 1.0f);

    drawDDALine(250, 130, 1030, 130);
    drawDDALine(1030, 130, 1030, 590);
    drawDDALine(1030, 590, 250, 590);
    drawDDALine(250, 590, 250, 130);

    glDisable(GL_BLEND);


    // ---------------------------------------------------------
    // TITLE
    // ---------------------------------------------------------

    glColor3f(
        0.95f,
        0.78f,
        0.25f
    );

    drawText5x7(
        "TECHNICIAN'S NOTE",
        315.0f,
        170.0f,
        3.0f
    );


    // ---------------------------------------------------------
    // PUZZLE QUESTION
    // ---------------------------------------------------------

    glColor3f(
        0.72f,
        0.80f,
        0.82f
    );

    drawText5x7(
        "FOUR SIGNALS. ONE CORRECT ORDER.",
        315.0f,
        225.0f,
        2.2f
    );

    drawText5x7(
        "THE SEQUENCE IS HIDDEN IN THE ROOM.",
        315.0f,
        265.0f,
        2.2f
    );


    // ---------------------------------------------------------
    // CLUES
    // ---------------------------------------------------------

    glColor3f(
        0.64f,
        0.74f,
        0.76f
    );

    drawText5x7(
        "THE FIRST STOPS WHAT APPROACHES.",
        315.0f,
        315.0f,
        2.0f
    );

    drawText5x7(
        "THE SECOND BELONGS TO THE OPEN SKY.",
        315.0f,
        350.0f,
        2.0f
    );

    drawText5x7(
        "THE THIRD DEMANDS CAUTION.",
        315.0f,
        385.0f,
        2.0f
    );

    drawText5x7(
        "THE LAST MEANS YOU MAY PROCEED.",
        315.0f,
        420.0f,
        2.0f
    );


    // ---------------------------------------------------------
    // FOUR EMPTY INPUT BOXES
    // ---------------------------------------------------------
    // These boxes are deliberately empty before the player types.
    // The colours are shown only as small reference indicators
    // above the input area; the letters themselves are not printed.
    //
    // The player's first typed letter appears in box 1,
    // second in box 2, third in box 3 and fourth in box 4.
    // ---------------------------------------------------------

    const float boxX[] =
    {
        350.0f,
        495.0f,
        640.0f,
        785.0f
    };

    const float boxWidth = 95.0f;
    const float boxTop = 445.0f;
    const float boxBottom = 525.0f;

    for (int i = 0; i < 4; ++i)
    {
        const float x = boxX[i];

        // Completely empty dark box.
        glColor3f(
            0.10f,
            0.14f,
            0.16f
        );

        drawScanlineFilledRectangle(
            x,
            boxTop,
            x + boxWidth,
            boxBottom
        );

        // Box outline.
        glColor3f(
            0.40f,
            0.47f,
            0.50f
        );

        drawDDALine(
            x,
            boxTop,
            x + boxWidth,
            boxTop
        );

        drawDDALine(
            x + boxWidth,
            boxTop,
            x + boxWidth,
            boxBottom
        );

        drawDDALine(
            x + boxWidth,
            boxBottom,
            x,
            boxBottom
        );

        drawDDALine(
            x,
            boxBottom,
            x,
            boxTop
        );


        // -----------------------------------------------------
        // PLAYER INPUT
        // -----------------------------------------------------
        // Nothing is drawn when this box has not been entered.
        // Once the player types a letter, show that exact letter
        // prominently in green inside the corresponding box.
        // -----------------------------------------------------

        if (enteredSequence != nullptr &&
            i < enteredCount &&
            enteredSequence[i] != '\0')
        {
            glColor3f(
                0.10f,
                0.90f,
                0.28f
            );

            drawChar5x7(
                enteredSequence[i],
                x + 35.0f,
                465.0f,
                4.0f
            );
        }
    }


    // ---------------------------------------------------------
    // INSTRUCTION / ERROR MESSAGE
    // ---------------------------------------------------------

    if (error)
    {
        glColor3f(
            0.90f,
            0.15f,
            0.18f
        );

        drawText5x7(
            "WRONG ORDER - BACKSPACE TO RESET",
            315.0f,
            550.0f,
            2.0f
        );
    }
    else
    {
        glColor3f(
            0.38f,
            0.70f,
            0.72f
        );

        drawText5x7(
            "USE B,G,R,Y KEYS",
            470.0f,
            550.0f,
            2.0f
        );
    }


    // ---------------------------------------------------------
    // CONFIRM
    // ---------------------------------------------------------

    glColor3f(
        0.72f,
        0.78f,
        0.80f
    );

    drawText5x7(
        "ENTER TO CONFIRM",
        500.0f,
        575.0f,
        2.0f
    );
}

// =============================================================
// ROOM 2 — ELECTRICAL ROOM
// =============================================================

// =============================================================
// ROOM 2 — ELECTRICAL ROOM
// =============================================================
// The layout below follows the supplied reference image:
//
//   FUSE BOX ---- red/blue cables ---- GENERATOR ---- yellow ---- PANEL
//
//   SWITCHES                 WORKBENCH                 MAIN SWITCH
//
//                                                        EXIT DOOR
//                                                        CRATES
//
// The left wall contains a real doorway so the player can see
// exactly where Room 1 leads into Room 2.
// =============================================================

void Renderer::drawFuseBox(bool inserted)
{
    const float left = 120.0f;
    const float top = 87.0f;
    const float right = 232.0f;
    const float bottom = 218.0f;

    // Outer housing.
    glColor3f(0.11f, 0.14f, 0.16f);
    drawScanlineFilledRectangle(left, top, right, bottom);

    glColor3f(0.56f, 0.61f, 0.65f);
    drawDDALine(left, top, right, top);
    drawDDALine(right, top, right, bottom);
    drawDDALine(right, bottom, left, bottom);
    drawDDALine(left, bottom, left, top);

    // Raised inner housing.
    glColor3f(0.16f, 0.20f, 0.23f);
    drawScanlineFilledRectangle(135.0f, 102.0f, 218.0f, 202.0f);

    glColor3f(0.36f, 0.41f, 0.45f);
    drawDDALine(135, 102, 218, 102);
    drawDDALine(218, 102, 218, 202);
    drawDDALine(218, 202, 135, 202);
    drawDDALine(135, 202, 135, 102);

    // Three fuse/switch sockets.
    for (int i = 0; i < 3; ++i)
    {
        const float y = 122.0f + i * 28.0f;

        glColor3f(0.045f, 0.06f, 0.07f);
        drawScanlineFilledRectangle(151.0f, y, 183.0f, y + 18.0f);

        glColor3f(0.58f, 0.63f, 0.67f);
        drawDDALine(151, y, 183, y);
        drawDDALine(183, y, 183, y + 18);
        drawDDALine(183, y + 18, 151, y + 18);
        drawDDALine(151, y + 18, 151, y);

        glColor3f(0.76f, 0.79f, 0.81f);
        drawScanlineFilledRectangle(158.0f, y + 5.0f,
                                    169.0f, y + 13.0f);
    }

    // Red warning/status lamp.
    glColor3f(0.82f, 0.06f, 0.10f);
    drawScanlineFilledRectangle(196, 140, 207, 158);

    // Main fuse / inserted state.
    glColor3f(inserted ? 0.10f : 0.08f,
              inserted ? 0.82f : 0.18f,
              inserted ? 0.34f : 0.20f);
    drawScanlineFilledRectangle(194, 171, 208, 190);

    glColor3f(0.70f, 0.74f, 0.77f);
    drawDDALine(194, 171, 208, 171);
    drawDDALine(208, 171, 208, 190);
    drawDDALine(208, 190, 194, 190);
    drawDDALine(194, 190, 194, 171);
}


void Renderer::drawCableSystem(bool connected)
{
    // ---------------------------------------------------------
    // RED CABLE: FUSE BOX -> GENERATOR
    // ---------------------------------------------------------
    glColor3f(0.86f, 0.08f, 0.10f);
    drawDDALine(232, 118, 280, 118);
    drawDDALine(280, 118, 318, 134);
    drawDDALine(318, 134, 445, 134);

    // ---------------------------------------------------------
    // BLUE CABLE: FUSE BOX -> GENERATOR
    // ---------------------------------------------------------
    glColor3f(0.08f, 0.38f, 0.88f);
    drawDDALine(232, 166, 280, 166);
    drawDDALine(280, 166, 318, 154);
    drawDDALine(318, 154, 445, 154);

    // ---------------------------------------------------------
    // YELLOW CABLE: GENERATOR -> ELECTRICAL PANEL
    // ---------------------------------------------------------
    glColor3f(0.95f, 0.68f, 0.05f);
    drawDDALine(662, 132, 722, 132);
    drawDDALine(722, 132, 755, 113);
    drawDDALine(755, 113, 881, 113);

    // Connection terminals.
    glColor3f(0.28f, 0.32f, 0.35f);
    drawScanlineFilledRectangle(311, 126, 322, 140);
    drawScanlineFilledRectangle(311, 147, 322, 161);
    drawScanlineFilledRectangle(746, 106, 757, 120);

    // Small status lamps show whether the cable stage is solved.
    glColor3f(connected ? 0.10f : 0.82f,
              connected ? 0.85f : 0.10f,
              0.18f);
    drawMidpointCircle(317, 133, 4);

    glColor3f(connected ? 0.10f : 0.82f,
              connected ? 0.85f : 0.10f,
              0.18f);
    drawMidpointCircle(317, 154, 4);

    glColor3f(connected ? 0.10f : 0.82f,
              connected ? 0.85f : 0.10f,
              0.18f);
    drawMidpointCircle(752, 113, 4);
}


void Renderer::drawGenerator(bool powered)
{
    const float left = 445.0f;
    const float top = 77.0f;
    const float right = 662.0f;
    const float bottom = 222.0f;

    // Exhaust pipes.
    glColor3f(0.34f, 0.39f, 0.43f);
    drawDDALine(548, 77, 548, 32);
    drawDDALine(548, 32, 560, 32);
    drawDDALine(560, 32, 560, 77);

    drawDDALine(558, 77, 558, 22);
    drawDDALine(558, 22, 570, 22);
    drawDDALine(570, 22, 570, 77);

    // Main generator housing.
    glColor3f(0.14f, 0.18f, 0.21f);
    drawScanlineFilledRectangle(left, top, right, bottom);

    glColor3f(0.52f, 0.58f, 0.63f);
    drawDDALine(left, top, right, top);
    drawDDALine(right, top, right, bottom);
    drawDDALine(right, bottom, left, bottom);
    drawDDALine(left, bottom, left, top);

    // Raised inner panel.
    glColor3f(0.20f, 0.25f, 0.29f);
    drawScanlineFilledRectangle(466, 92, 642, 207);

    glColor3f(0.33f, 0.39f, 0.44f);
    drawDDALine(466, 92, 642, 92);
    drawDDALine(642, 92, 642, 207);
    drawDDALine(642, 207, 466, 207);
    drawDDALine(466, 207, 466, 92);

    // Central lightning symbol.
    // When the generator is powered, the lightning symbol continuously
    // grows and shrinks around its centre. This is an explicit scaling
    // transformation while the generator housing stays fixed.
    glColor3f(0.96f, 0.72f, 0.05f);

    float lightningScale = 1.0f;

    if (powered)
    {
        // Smooth scale range: approximately 0.82 -> 1.18 -> 0.82.
        const float pulse =
            0.5f + 0.5f * std::sin(generatorPulseTime * 3.0f);

        lightningScale = 0.82f + 0.36f * pulse;
    }

    glPushMatrix();

    // Scale around the centre of the lightning symbol.
    glTranslatef(560.0f, 143.0f, 0.0f);
    glScalef(lightningScale, lightningScale, 1.0f);
    glTranslatef(-560.0f, -143.0f, 0.0f);

    drawDDALine(558, 111, 543, 142);
    drawDDALine(543, 142, 558, 142);
    drawDDALine(558, 142, 548, 176);
    drawDDALine(548, 176, 578, 137);
    drawDDALine(578, 137, 562, 137);
    drawDDALine(562, 137, 572, 111);

    glPopMatrix();

    // Status lamp.
    glColor3f(powered ? 0.10f : 0.82f,
              powered ? 0.86f : 0.08f,
              0.18f);
    drawMidpointCircle(622, 97, 7);

    // Small generator bolts/details.
    glColor3f(0.45f, 0.50f, 0.54f);
    drawDDALine(477, 105, 492, 105);
    drawDDALine(477, 112, 487, 112);
    drawDDALine(617, 105, 632, 105);
    drawDDALine(622, 112, 632, 112);
}


void Renderer::drawElectricalPanel(bool active)
{
    const float left = 881.0f;
    const float top = 87.0f;
    const float right = 982.0f;
    const float bottom = 218.0f;

    glColor3f(0.12f, 0.16f, 0.18f);
    drawScanlineFilledRectangle(left, top, right, bottom);

    glColor3f(0.55f, 0.61f, 0.66f);
    drawDDALine(left, top, right, top);
    drawDDALine(right, top, right, bottom);
    drawDDALine(right, bottom, left, bottom);
    drawDDALine(left, bottom, left, top);

    glColor3f(0.20f, 0.25f, 0.28f);
    drawScanlineFilledRectangle(896, 102, 967, 202);

    glColor3f(0.35f, 0.40f, 0.44f);
    drawDDALine(896, 102, 967, 102);
    drawDDALine(967, 102, 967, 202);
    drawDDALine(967, 202, 896, 202);
    drawDDALine(896, 202, 896, 102);

    // Three electrical breakers/status rows.
    for (int i = 0; i < 3; ++i)
    {
        const float y = 118.0f + i * 28.0f;

        glColor3f(0.045f, 0.06f, 0.07f);
        drawScanlineFilledRectangle(916, y, 947, y + 18);

        glColor3f(0.35f, 0.40f, 0.44f);
        drawDDALine(916, y, 947, y);
        drawDDALine(947, y, 947, y + 18);
        drawDDALine(947, y + 18, 916, y + 18);
        drawDDALine(916, y + 18, 916, y);

        glColor3f(active ? 0.10f : 0.82f,
                  active ? 0.86f : 0.08f,
                  0.18f);
        drawScanlineFilledRectangle(936, y + 6, 943, y + 12);
    }
}


void Renderer::drawRoom2ExitDoor(bool unlocked)
{
    const float left = 1040.0f;
    const float right = 1145.0f;
    const float top = 207.0f;
    const float bottom = 403.0f;

    // Door frame.
    glColor3f(0.08f, 0.11f, 0.13f);
    drawScanlineFilledRectangle(left, top, right, bottom);

    glColor3f(0.42f, 0.48f, 0.52f);
    drawDDALine(left, top, right, top);
    drawDDALine(right, top, right, bottom);
    drawDDALine(right, bottom, left, bottom);
    drawDDALine(left, bottom, left, top);

    // Door leaf.
    glColor3f(0.12f, 0.17f, 0.19f);
    drawScanlineFilledRectangle(1069, 227, 1127, 382);

    glColor3f(0.33f, 0.39f, 0.43f);
    drawDDALine(1069, 227, 1127, 227);
    drawDDALine(1127, 227, 1127, 382);
    drawDDALine(1127, 382, 1069, 382);
    drawDDALine(1069, 382, 1069, 227);

    // Lock/status indicator.
    glColor3f(unlocked ? 0.10f : 0.82f,
              unlocked ? 0.86f : 0.08f,
              0.18f);
    drawScanlineFilledRectangle(1098, 287, 1114, 307);

    // Door handle.
    glColor3f(0.68f, 0.72f, 0.75f);
    drawDDALine(1107, 323, 1107, 348);
}


void Renderer::drawRoom2(
    bool fuseInserted,
    bool cablesConnected,
    bool generatorPowered,
    bool panelActivated,
    bool mainSwitchActivated,
    bool exitDoorUnlocked
)
{
    // =========================================================
    // DARK INDUSTRIAL FLOOR
    // =========================================================
    glColor3f(0.035f, 0.045f, 0.052f);
    drawScanlineFilledRectangle(
        ROOM_LEFT + 1.0f,
        ROOM_TOP + 1.0f,
        ROOM_RIGHT - 1.0f,
        ROOM_BOTTOM - 1.0f
    );

    // Subtle floor grid.
    glColor3f(0.055f, 0.068f, 0.076f);
    for (int x = static_cast<int>(ROOM_LEFT + 20); x < ROOM_RIGHT; x += 40)
        drawDDALine(static_cast<float>(x), ROOM_TOP + 5,
                    static_cast<float>(x), ROOM_BOTTOM - 5);

    for (int y = static_cast<int>(ROOM_TOP + 20); y < ROOM_BOTTOM; y += 40)
        drawDDALine(ROOM_LEFT + 5, static_cast<float>(y),
                    ROOM_RIGHT - 5, static_cast<float>(y));

    // =========================================================
    // WALLS — LEFT WALL HAS AN OPEN ENTRY
    // =========================================================
    glColor3f(0.48f, 0.53f, 0.57f);

    drawDDALine(ROOM_LEFT, ROOM_TOP,
                ROOM_RIGHT, ROOM_TOP);

    drawDDALine(ROOM_RIGHT, ROOM_TOP,
                ROOM_RIGHT, ROOM_BOTTOM);

    drawDDALine(ROOM_RIGHT, ROOM_BOTTOM,
                ROOM_LEFT, ROOM_BOTTOM);

    // Left wall segments leave a 100-pixel doorway.
    drawDDALine(ROOM_LEFT, ROOM_TOP,
                ROOM_LEFT, 430.0f);

    drawDDALine(ROOM_LEFT, 530.0f,
                ROOM_LEFT, ROOM_BOTTOM);

    // =========================================================
    // ROOM 1 -> ROOM 2 ENTRY FRAME
    // =========================================================
    // The opening is deliberately obvious from the room interior.
    glColor3f(0.25f, 0.34f, 0.38f);

    drawDDALine(ROOM_LEFT, 430, 105, 430);
    drawDDALine(ROOM_LEFT, 530, 105, 530);

    drawDDALine(105, 430, 105, 530);

    // Threshold / floor strips.
    glColor3f(0.12f, 0.72f, 0.42f);
    drawDDALine(70, 448, 105, 448);
    drawDDALine(70, 465, 105, 465);
    drawDDALine(70, 482, 105, 482);
    drawDDALine(70, 499, 105, 499);
    drawDDALine(70, 516, 105, 516);

    // Direction arrows point inward toward Room 2.
    glColor3f(0.16f, 0.82f, 0.46f);
    drawDDALine(73, 480, 93, 480);
    drawDDALine(93, 480, 84, 472);
    drawDDALine(93, 480, 84, 488);

    // =========================================================
    // MAIN OBJECTS
    // =========================================================
    drawFuseBox(fuseInserted);
    drawCableSystem(cablesConnected);
    drawGenerator(generatorPowered);
    drawElectricalPanel(panelActivated);

    // =========================================================
    // SWITCH BANK — LEFT LOWER SIDE
    // =========================================================
    glColor3f(0.10f, 0.14f, 0.16f);
    drawScanlineFilledRectangle(55, 267, 142, 428);

    glColor3f(0.48f, 0.54f, 0.58f);
    drawDDALine(55, 267, 142, 267);
    drawDDALine(142, 267, 142, 428);
    drawDDALine(142, 428, 55, 428);
    drawDDALine(55, 428, 55, 267);

    for (int i = 0; i < 3; ++i)
    {
        const float y = 287.0f + i * 50.0f;

        glColor3f(0.06f, 0.08f, 0.09f);
        drawScanlineFilledRectangle(76, y, 121, y + 32);

        glColor3f(0.40f, 0.46f, 0.50f);
        drawDDALine(76, y, 121, y);
        drawDDALine(121, y, 121, y + 32);
        drawDDALine(121, y + 32, 76, y + 32);
        drawDDALine(76, y + 32, 76, y);

        glColor3f(0.84f, 0.08f, 0.12f);
        drawScanlineFilledRectangle(85, y + 8, 98, y + 22);
    }

    // =========================================================
    // WORKBENCH
    // =========================================================
    glColor3f(0.24f, 0.13f, 0.055f);
    drawScanlineFilledRectangle(366, 323, 657, 404);

    glColor3f(0.63f, 0.38f, 0.14f);
    drawDDALine(366, 323, 657, 323);
    drawDDALine(657, 323, 657, 404);
    drawDDALine(657, 404, 366, 404);
    drawDDALine(366, 404, 366, 323);

    glColor3f(0.33f, 0.19f, 0.08f);
    drawScanlineFilledRectangle(378, 335, 645, 392);

    // Workbench legs.
    glColor3f(0.34f, 0.20f, 0.08f);
    drawDDALine(386, 404, 386, 423);
    drawDDALine(637, 404, 637, 423);

    // Wrench.
    glColor3f(0.72f, 0.75f, 0.77f);
    drawDDALine(430, 365, 466, 345);
    drawDDALine(430, 365, 438, 374);
    drawDDALine(430, 365, 439, 356);
    drawDDALine(466, 345, 472, 337);
    drawDDALine(466, 345, 474, 347);

    // Screwdriver.
    glColor3f(0.88f, 0.12f, 0.12f);
    drawDDALine(500, 374, 534, 343);
    glColor3f(0.72f, 0.75f, 0.77f);
    drawDDALine(534, 343, 541, 336);
    drawDDALine(530, 347, 536, 353);

    // Small toolbox.
    glColor3f(0.10f, 0.16f, 0.19f);
    drawScanlineFilledRectangle(561, 342, 612, 375);
    glColor3f(0.45f, 0.51f, 0.55f);
    drawDDALine(561, 342, 612, 342);
    drawDDALine(612, 342, 612, 375);
    drawDDALine(612, 375, 561, 375);
    drawDDALine(561, 375, 561, 342);
    drawDDALine(573, 338, 600, 338);
    drawDDALine(573, 338, 573, 342);
    drawDDALine(600, 338, 600, 342);

    // =========================================================
    // MAIN SWITCH
    // =========================================================
    glColor3f(0.07f, 0.09f, 0.10f);
    drawScanlineFilledRectangle(791, 312, 867, 413);

    glColor3f(0.92f, 0.68f, 0.05f);
    drawDDALine(791, 312, 867, 312);
    drawDDALine(867, 312, 867, 413);
    drawDDALine(867, 413, 791, 413);
    drawDDALine(791, 413, 791, 312);

    glColor3f(0.16f, 0.21f, 0.23f);
    drawScanlineFilledRectangle(812, 337, 846, 399);

    // Switch handle.
    glColor3f(0.78f, 0.80f, 0.80f);
    drawScanlineFilledRectangle(825, 348, 833, 379);

    // Red top cap and status lamp.
    glColor3f(0.84f, 0.08f, 0.12f);
    drawScanlineFilledRectangle(808, 321, 850, 331);

    glColor3f(mainSwitchActivated ? 0.10f : 0.84f,
              mainSwitchActivated ? 0.86f : 0.08f,
              0.18f);
    drawScanlineFilledRectangle(825, 367, 835, 383);

    // =========================================================
    // EXIT DOOR
    // =========================================================
    drawRoom2ExitDoor(exitDoorUnlocked);

    // =========================================================
    // STORAGE CRATES — BOTTOM RIGHT
    // =========================================================
    glColor3f(0.27f, 0.16f, 0.07f);
    drawScanlineFilledRectangle(1010, 447, 1065, 501);

    glColor3f(0.57f, 0.34f, 0.12f);
    drawDDALine(1010, 447, 1065, 447);
    drawDDALine(1065, 447, 1065, 501);
    drawDDALine(1065, 501, 1010, 501);
    drawDDALine(1010, 501, 1010, 447);
    drawDDALine(1012, 449, 1063, 499);
    drawDDALine(1063, 449, 1012, 499);

    glColor3f(0.27f, 0.16f, 0.07f);
    drawScanlineFilledRectangle(1065, 488, 1120, 540);

    glColor3f(0.57f, 0.34f, 0.12f);
    drawDDALine(1065, 488, 1120, 488);
    drawDDALine(1120, 488, 1120, 540);
    drawDDALine(1120, 540, 1065, 540);
    drawDDALine(1065, 540, 1065, 488);
    drawDDALine(1067, 490, 1118, 538);
    drawDDALine(1118, 490, 1067, 538);

    // =========================================================
    // SMALL PUZZLE STATUS CHAIN
    // =========================================================
    // Four small lamps give visual feedback without replacing
    // the actual object interactions.
    const float statusY = 610.0f;
    const float statusStart = 510.0f;

    glColor3f(fuseInserted ? 0.10f : 0.82f,
              fuseInserted ? 0.85f : 0.08f,
              0.18f);
    drawMidpointCircle(static_cast<int>(statusStart),
                       static_cast<int>(statusY), 5);

    glColor3f(cablesConnected ? 0.10f : 0.82f,
              cablesConnected ? 0.85f : 0.08f,
              0.18f);
    drawMidpointCircle(static_cast<int>(statusStart + 35),
                       static_cast<int>(statusY), 5);

    glColor3f(generatorPowered ? 0.10f : 0.82f,
              generatorPowered ? 0.85f : 0.08f,
              0.18f);
    drawMidpointCircle(static_cast<int>(statusStart + 70),
                       static_cast<int>(statusY), 5);

    glColor3f(panelActivated ? 0.10f : 0.82f,
              panelActivated ? 0.85f : 0.08f,
              0.18f);
    drawMidpointCircle(static_cast<int>(statusStart + 105),
                       static_cast<int>(statusY), 5);
}
