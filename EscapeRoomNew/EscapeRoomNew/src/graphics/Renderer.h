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

    void resize(int framebufferWidth, int framebufferHeight);

    // =========================================================
    // ROOM 1 / ROOM 2 RENDERING
    // =========================================================

    void drawRoom(bool doorUnlocked, float doorOpenAmount);

    // =========================================================
    // ROOM 2 RENDERING
    // =========================================================

    void drawRoom2(
        bool fuseInserted,
        bool cablesConnected,
        bool generatorPowered,
        bool panelActivated,
        bool mainSwitchActivated,
        bool exitDoorUnlocked
    );

    void drawRoom2Message(int messageType);

    void drawRoom2Puzzle(
        const char* enteredSequence,
        int enteredCount,
        bool error
    );

    // =========================================================
    // PLAYER
    // =========================================================

    void drawPlayer(float x, float y);

    // =========================================================
    // INTERACTION PROMPT
    // =========================================================

    void drawInteractionPrompt(float x, float y);

    // =========================================================
    // PC PUZZLE
    // =========================================================

    void drawComputerPuzzle(
        int enteredValue,
        int digitsEntered,
        bool error
    );

    // =========================================================
    // COMPUTER SOLVED MESSAGE
    // =========================================================

    void drawComputerSolvedMessage();

    // =========================================================
    // ACCESS CARD MESSAGE
    // =========================================================

    void drawAccessCardMessage();

    // =========================================================
    // ACCESS GRANTED MESSAGE
    // =========================================================

    void drawAccessGrantedMessage();

    // =========================================================
    // CCTV
    // =========================================================

    void updateCCTV(double deltaTime);

    // Updates the Room 2 generator lightning-symbol scaling animation.
    void updateGeneratorAnimation(double deltaTime);

private:

    GLFWwindow* window;

    int framebufferWidth = 1280;
    int framebufferHeight = 720;

    void updateViewport();

    // =========================================================
    // FUNDAMENTAL COMPUTER GRAPHICS ALGORITHMS
    // =========================================================

    void drawDDALine(float x1, float y1, float x2, float y2);

    void drawMidpointCircle(
        int centerX,
        int centerY,
        int radius
    );

    void drawScanlineFilledRectangle(
        float left,
        float top,
        float right,
        float bottom
    );

    float cctvAngle = 145.0f;
    float cctvSweepDirection = 1.0f;

    // Time accumulator used for the generator lightning-symbol pulse.
    float generatorPulseTime = 0.0f;

    void drawFilledRectangle(
        float left,
        float top,
        float right,
        float bottom
    );

    void drawPuzzleDigit(
        int digit,
        float x,
        float y,
        float scale
    );

    // =========================================================
    // ROOM 2 MESSAGE / PUZZLE TEXT
    // =========================================================

    // Small bitmap font used for Room 2 messages and puzzle UI.
    void drawChar5x7(
        char character,
        float x,
        float y,
        float scale
    );

    void drawText5x7(
        const char* text,
        float x,
        float y,
        float scale
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

    void drawLockedDoor(
        bool doorUnlocked,
        float doorOpenAmount
    );

    // =========================================================
    // ROOM 2 OBJECTS
    // =========================================================

    void drawFuseBox(bool inserted);
    void drawCableSystem(bool connected);
    void drawGenerator(bool powered);
    void drawElectricalPanel(bool active);
    void drawRoom2ExitDoor(bool unlocked);
};
