#pragma once

#include <GLFW/glfw3.h>

class Renderer;

class Game
{
public:
    explicit Game(GLFWwindow* window);
    ~Game();

    // =========================================================
    // GAME LOOP
    // =========================================================

    void update(double deltaTime);
    void render();

    // =========================================================
    // WINDOW / RESIZING
    // =========================================================

    void resize(int width, int height);

    // =========================================================
    // KEYBOARD INPUT
    // =========================================================

    void keyPressed(int key);
    void keyReleased(int key);

private:

    // =========================================================
    // WINDOW / RENDERER
    // =========================================================

    GLFWwindow* window;
    Renderer* renderer;

    // =========================================================
    // KEYBOARD STATE
    // =========================================================

    bool keyState[GLFW_KEY_LAST + 1]{};

    // =========================================================
    // PLAYER
    // =========================================================

    double playerX = 180.0;
    double playerY = 560.0;

    static constexpr double PLAYER_SPEED = 250.0;
    static constexpr double PLAYER_RADIUS = 8.0;

    // =========================================================
    // PLAYER MOVEMENT
    // =========================================================

    void movePlayer(double deltaX, double deltaY);

    // =========================================================
    // COLLISION
    // =========================================================

    bool checkPlayerCollision(double newX, double newY) const;

    bool checkRectangleCollision(
        double playerX,
        double playerY,
        double playerRadius,
        double left,
        double top,
        double right,
        double bottom
    ) const;

    // =========================================================
    // ROOM BOUNDS
    // =========================================================

    bool insideRoom(double x, double y) const;

    // =========================================================
    // INTERACTION SYSTEM
    // =========================================================

    void checkInteraction();
    bool isNearComputer() const;

    bool eWasPressed = false;

    // =========================================================
    // ROOM 1 INTERACTION STATE
    // =========================================================

    bool computerInteracted = false;

    // =========================================================
    // PC PUZZLE STATE
    // =========================================================

    bool computerOpen = false;
    bool computerSolved = false;

    int computerInput = 0;
    int computerDigitsEntered = 0;
    bool computerError = false;

    // =========================================================
    // COMPUTER SOLVED MESSAGE
    // =========================================================

    bool computerSolvedMessage = false;

    // =========================================================
    // CABINET / ACCESS CARD STATE
    // =========================================================

    bool accessCardObtained = false;
    bool accessCardMessage = false;

    bool isNearCabinet() const;

    // =========================================================
    // LOCKED DOOR STATE
    // =========================================================

    bool isNearLockedDoor() const;

    bool exitDoorUnlocked = false;
    bool accessGrantedMessage = false;

    bool doorOpening = false;
    bool doorOpen = false;
    double doorOpenAmount = 0.0;

    static constexpr double DOOR_OPEN_DURATION = 1.0;

    // =========================================================
    // CURRENT ROOM
    // =========================================================

    int currentRoom = 1;

    // =========================================================
    // ROOM 2 INTERACTION SYSTEM
    // =========================================================

    bool isNearRoom2FuseBox() const;
    bool isNearRoom2Workbench() const;
    bool isNearRoom2CableSystem() const;
    bool isNearRoom2Generator() const;
    bool isNearRoom2Panel() const;
    bool isNearRoom2MainSwitch() const;
    bool isNearRoom2ExitDoor() const;

    bool nearRectangle(
        double playerX,
        double playerY,
        double left,
        double top,
        double right,
        double bottom,
        double distance
    ) const;

    // =========================================================
    // ROOM 2 PUZZLE STATE
    // =========================================================

    bool room2FuseInserted = false;
    bool room2CablesConnected = false;
    bool room2GeneratorPowered = false;
    bool room2PanelActivated = false;
    bool room2MainSwitchActivated = false;
    bool room2ExitDoorUnlocked = false;
    bool room2Finished = false;

    // Room 2 messages. 0 = none, 1 = entrance, 2 = fuse,
    // 3 = cables, 4 = generator, 5 = panel, 6 = main switch,
    // 7 = exit door, 8 = finished.
    bool room2Message = false;
    int room2MessageType = 0;

    // Workbench cable-order puzzle.
    bool room2PuzzleOpen = false;
    bool room2PuzzleSolved = false;
    bool room2PuzzleError = false;
    int room2PuzzleStep = 0;
    int room2PuzzleInputCount = 0;

    // Letters entered by the player, shown live in the puzzle boxes.
    char room2PuzzleInput[5] = { '\0', '\0', '\0', '\0', '\0' };
};
