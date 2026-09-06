#include "Renderer.h"

#include <algorithm>
#include <cmath>


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

void Renderer::drawPlant()
{
    // ---------------------------------------------------------
    // PLANT POSITION
    // Upper-left corner of the room
    // ---------------------------------------------------------

    const float centerX = 105.0f;
    const float potTop  = 170.0f;


    // ---------------------------------------------------------
    // STEM
    // ---------------------------------------------------------

    glColor3f(
        0.18f,
        0.48f,
        0.18f
    );

    drawDDALine(
        centerX,
        145.0f,
        centerX,
        potTop
    );


    // ---------------------------------------------------------
    // LEFT LEAF
    // ---------------------------------------------------------

    glColor3f(
        0.12f,
        0.55f,
        0.18f
    );

    glBegin(GL_POLYGON);

    glVertex2f(centerX, 150.0f);
    glVertex2f(centerX - 18.0f, 138.0f);
    glVertex2f(centerX - 27.0f, 120.0f);
    glVertex2f(centerX - 10.0f, 125.0f);
    glVertex2f(centerX, 140.0f);

    glEnd();


    // ---------------------------------------------------------
    // RIGHT LEAF
    // ---------------------------------------------------------

    glColor3f(
        0.16f,
        0.62f,
        0.20f
    );

    glBegin(GL_POLYGON);

    glVertex2f(centerX, 150.0f);
    glVertex2f(centerX + 18.0f, 138.0f);
    glVertex2f(centerX + 27.0f, 120.0f);
    glVertex2f(centerX + 10.0f, 125.0f);
    glVertex2f(centerX, 140.0f);

    glEnd();


    // ---------------------------------------------------------
    // TOP LEAF
    // ---------------------------------------------------------

    glColor3f(
        0.18f,
        0.68f,
        0.22f
    );

    glBegin(GL_POLYGON);

    glVertex2f(centerX, 145.0f);
    glVertex2f(centerX - 11.0f, 130.0f);
    glVertex2f(centerX - 8.0f, 108.0f);
    glVertex2f(centerX, 118.0f);
    glVertex2f(centerX + 8.0f, 108.0f);
    glVertex2f(centerX + 11.0f, 130.0f);

    glEnd();


    // ---------------------------------------------------------
    // LEFT LOWER LEAF
    // ---------------------------------------------------------

    glColor3f(
        0.10f,
        0.48f,
        0.15f
    );

    glBegin(GL_POLYGON);

    glVertex2f(centerX - 2.0f, 150.0f);
    glVertex2f(centerX - 24.0f, 146.0f);
    glVertex2f(centerX - 30.0f, 132.0f);
    glVertex2f(centerX - 12.0f, 137.0f);

    glEnd();


    // ---------------------------------------------------------
    // RIGHT LOWER LEAF
    // ---------------------------------------------------------

    glColor3f(
        0.13f,
        0.58f,
        0.18f
    );

    glBegin(GL_POLYGON);

    glVertex2f(centerX + 2.0f, 150.0f);
    glVertex2f(centerX + 24.0f, 146.0f);
    glVertex2f(centerX + 30.0f, 132.0f);
    glVertex2f(centerX + 12.0f, 137.0f);

    glEnd();


    // ---------------------------------------------------------
    // POT
    // ---------------------------------------------------------

    glColor3f(
        0.32f,
        0.18f,
        0.08f
    );

    glBegin(GL_POLYGON);

    glVertex2f(centerX - 14.0f, potTop);
    glVertex2f(centerX + 14.0f, potTop);
    glVertex2f(centerX + 10.0f, potTop + 20.0f);
    glVertex2f(centerX - 10.0f, potTop + 20.0f);

    glEnd();


    // ---------------------------------------------------------
    // POT RIM
    // ---------------------------------------------------------

    glColor3f(
        0.55f,
        0.32f,
        0.12f
    );

    drawScanlineFilledRectangle(
        centerX - 17.0f,
        potTop - 3.0f,
        centerX + 17.0f,
        potTop + 3.0f
    );


    // ---------------------------------------------------------
    // POT OUTLINE
    // ---------------------------------------------------------

    glColor3f(
        0.72f,
        0.45f,
        0.17f
    );

    drawDDALine(
        centerX - 14.0f,
        potTop,
        centerX + 14.0f,
        potTop
    );

    drawDDALine(
        centerX + 14.0f,
        potTop,
        centerX + 10.0f,
        potTop + 20.0f
    );

    drawDDALine(
        centerX + 10.0f,
        potTop + 20.0f,
        centerX - 10.0f,
        potTop + 20.0f
    );

    drawDDALine(
        centerX - 10.0f,
        potTop + 20.0f,
        centerX - 14.0f,
        potTop
    );


    // ---------------------------------------------------------
    // LEAF VEINS
    // ---------------------------------------------------------

    glColor3f(
        0.35f,
        0.80f,
        0.30f
    );

    drawDDALine(
        centerX,
        143.0f,
        centerX - 20.0f,
        125.0f
    );

    drawDDALine(
        centerX,
        143.0f,
        centerX + 20.0f,
        125.0f
    );

    drawDDALine(
        centerX,
        138.0f,
        centerX,
        116.0f
    );
}

// =============================================================
// ROOM 1 — CCTV
// =============================================================

void Renderer::drawCCTV()
{
    // ---------------------------------------------------------
    // CCTV POSITION
    // ---------------------------------------------------------

    const float cameraX = 1090.0f;
    const float cameraY = 135.0f;


    // ---------------------------------------------------------
    // SURVEILLANCE RANGE
    // Midpoint Circle Algorithm
    // ---------------------------------------------------------

    glColor3f(
        0.18f,
        0.32f,
        0.38f
    );

    drawMidpointCircle(
        static_cast<int>(cameraX),
        static_cast<int>(cameraY),
        115
    );


    // ---------------------------------------------------------
    // CAMERA MOUNT
    // ---------------------------------------------------------

    glColor3f(
        0.22f,
        0.25f,
        0.28f
    );

    drawScanlineFilledRectangle(
        cameraX - 14.0f,
        cameraY - 14.0f,
        cameraX + 10.0f,
        cameraY + 10.0f
    );


    // ---------------------------------------------------------
    // CAMERA BODY
    // ---------------------------------------------------------

    glColor3f(
        0.38f,
        0.42f,
        0.46f
    );

    glBegin(GL_POLYGON);

    glVertex2f(
        cameraX - 13.0f,
        cameraY - 10.0f
    );

    glVertex2f(
        cameraX + 7.0f,
        cameraY - 14.0f
    );

    glVertex2f(
        cameraX + 19.0f,
        cameraY - 2.0f
    );

    glVertex2f(
        cameraX + 13.0f,
        cameraY + 13.0f
    );

    glVertex2f(
        cameraX - 9.0f,
        cameraY + 10.0f
    );

    glEnd();


    // ---------------------------------------------------------
    // CAMERA LENS
    // ---------------------------------------------------------

    glColor3f(
        0.04f,
        0.07f,
        0.09f
    );

    drawMidpointCircle(
        static_cast<int>(cameraX + 11.0f),
        static_cast<int>(cameraY),
        5
    );


    // ---------------------------------------------------------
    // LENS INDICATOR
    // ---------------------------------------------------------

    glColor3f(
        0.10f,
        0.80f,
        0.65f
    );

    glBegin(GL_POINTS);

    glVertex2f(
        cameraX + 11.0f,
        cameraY - 1.0f
    );

    glEnd();


    // ---------------------------------------------------------
    // CAMERA OUTLINE
    // DDA
    // ---------------------------------------------------------

    glColor3f(
        0.65f,
        0.70f,
        0.74f
    );

    drawDDALine(
        cameraX - 13.0f,
        cameraY - 10.0f,
        cameraX + 7.0f,
        cameraY - 14.0f
    );

    drawDDALine(
        cameraX + 7.0f,
        cameraY - 14.0f,
        cameraX + 19.0f,
        cameraY - 2.0f
    );

    drawDDALine(
        cameraX + 19.0f,
        cameraY - 2.0f,
        cameraX + 13.0f,
        cameraY + 13.0f
    );

    drawDDALine(
        cameraX + 13.0f,
        cameraY + 13.0f,
        cameraX - 9.0f,
        cameraY + 10.0f
    );


    // ---------------------------------------------------------
    // SUPPORT ARM
    // ---------------------------------------------------------

    glColor3f(
        0.30f,
        0.34f,
        0.37f
    );

    drawDDALine(
        cameraX - 7.0f,
        cameraY - 10.0f,
        cameraX - 22.0f,
        cameraY - 24.0f
    );

    drawDDALine(
        cameraX - 22.0f,
        cameraY - 24.0f,
        cameraX - 34.0f,
        cameraY - 18.0f
    );


    // ---------------------------------------------------------
    // CAMERA VIEW DIRECTION
    // ---------------------------------------------------------

    glColor3f(
        0.45f,
        0.18f,
        0.18f
    );

    drawDDALine(
        cameraX + 15.0f,
        cameraY + 3.0f,
        980.0f,
        245.0f
    );

    drawDDALine(
        cameraX + 15.0f,
        cameraY + 3.0f,
        1120.0f,
        260.0f
    );
}

void Renderer::drawRoom()
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

    drawPlant();

    drawCCTV();
}