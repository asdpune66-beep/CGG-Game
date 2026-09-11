#include "Game.h"
#include "../graphics/Renderer.h"

#include <algorithm>
#include <cmath>


// =============================================================
// CONSTRUCTOR
// =============================================================

Game::Game(GLFWwindow* window)
    : window(window)
{
    renderer = new Renderer(window);
}

Game::~Game()
{
    delete renderer;
    renderer = nullptr;
}


// =============================================================
// WINDOW RESIZE
// =============================================================

void Game::resize(int width, int height)
{
    renderer->resize(width, height);
}


// =============================================================
// KEY PRESSED
// =============================================================

void Game::keyPressed(int key)
{
    // =========================================================
    // ROOM 2 MESSAGE
    // =========================================================
    if (currentRoom == 2 && room2Message)
    {
        if (key == GLFW_KEY_E || key == GLFW_KEY_ENTER)
        {
            room2Message = false;
            room2MessageType = 0;
        }
        return;
    }

    // =========================================================
    // ROOM 2 WORKBENCH PUZZLE
    // =========================================================
    if (currentRoom == 2 && room2PuzzleOpen)
    {
        // The player enters the colour letters directly.
        // Each typed letter is stored and displayed live in the
        // corresponding puzzle box.
        auto enterPuzzleLetter = [&](char letter, int expectedStep)
        {
            if (room2PuzzleStep < 0 || room2PuzzleStep >= 4)
            {
                room2PuzzleError = true;
                return;
            }

            room2PuzzleInput[room2PuzzleInputCount] = letter;
            ++room2PuzzleInputCount;
            room2PuzzleInput[room2PuzzleInputCount] = '\0';

            if (room2PuzzleStep == expectedStep)
            {
                ++room2PuzzleStep;
            }
            else
            {
                room2PuzzleStep = -1;
                room2PuzzleError = true;
            }
        };

        if (key == GLFW_KEY_R)
        {
            enterPuzzleLetter('R', 0);
            return;
        }

        if (key == GLFW_KEY_B)
        {
            enterPuzzleLetter('B', 1);
            return;
        }

        if (key == GLFW_KEY_Y)
        {
            enterPuzzleLetter('Y', 2);
            return;
        }

        if (key == GLFW_KEY_G)
        {
            enterPuzzleLetter('G', 3);
            return;
        }

        if (key == GLFW_KEY_BACKSPACE)
        {
            room2PuzzleStep = 0;
            room2PuzzleInputCount = 0;
            room2PuzzleError = false;
            room2PuzzleInput[0] = '\0';
            room2PuzzleInput[1] = '\0';
            room2PuzzleInput[2] = '\0';
            room2PuzzleInput[3] = '\0';
            room2PuzzleInput[4] = '\0';
            return;
        }

        if (key == GLFW_KEY_ENTER)
        {
            if (room2PuzzleStep == 4)
            {
                room2PuzzleSolved = true;
                room2PuzzleOpen = false;
                room2PuzzleError = false;
                room2Message = true;
                room2MessageType = 9;
            }
            else
            {
                room2PuzzleError = true;
            }
            return;
        }

        return;
    }

    // ---------------------------------------------------------
    // ACCESS CARD MESSAGE
    // ---------------------------------------------------------

    if (accessCardMessage)
    {
        // Keep the access-card message visible until
        // the player acknowledges it.
        if (key == GLFW_KEY_E ||
            key == GLFW_KEY_ENTER)
        {
            accessCardMessage = false;
        }

        return;
    }

    // ---------------------------------------------------------
    // ACCESS GRANTED MESSAGE
    // ---------------------------------------------------------

    if (accessGrantedMessage)
    {
        if (key == GLFW_KEY_E ||
            key == GLFW_KEY_ENTER)
        {
            // Close the ACCESS GRANTED message.
            accessGrantedMessage = false;

            // Start the physical door-opening animation.
            if (!doorOpen && !doorOpening)
            {
                doorOpening = true;
                doorOpenAmount = 0.0;
            }
        }

        return;
    }


    // ---------------------------------------------------------
    // COMPUTER SOLVED MESSAGE
    // ---------------------------------------------------------

    if (computerSolvedMessage)
    {
        // The computer has been solved. Keep the objective message
        // visible until the player acknowledges it.
        if (key == GLFW_KEY_E ||
            key == GLFW_KEY_ENTER)
        {
            computerSolvedMessage = false;
        }

        return;
    }


    if (computerOpen)
    {
        // -----------------------------------------------------
        // Number 0
        // -----------------------------------------------------

        if (key == GLFW_KEY_0)
        {
            if (computerDigitsEntered < 2)
            {
                computerInput =
                    computerInput * 10;

                computerDigitsEntered++;
            }

            return;
        }


        // -----------------------------------------------------
        // Number 1
        // -----------------------------------------------------

        if (key == GLFW_KEY_1)
        {
            if (computerDigitsEntered < 2)
            {
                computerInput =
                    computerInput * 10 + 1;

                computerDigitsEntered++;
            }

            return;
        }


        // -----------------------------------------------------
        // Number 2
        // -----------------------------------------------------

        if (key == GLFW_KEY_2)
        {
            if (computerDigitsEntered < 2)
            {
                computerInput =
                    computerInput * 10 + 2;

                computerDigitsEntered++;
            }

            return;
        }


        // -----------------------------------------------------
        // Number 3
        // -----------------------------------------------------

        if (key == GLFW_KEY_3)
        {
            if (computerDigitsEntered < 2)
            {
                computerInput =
                    computerInput * 10 + 3;

                computerDigitsEntered++;
            }

            return;
        }


        // -----------------------------------------------------
        // Number 4
        // -----------------------------------------------------

        if (key == GLFW_KEY_4)
        {
            if (computerDigitsEntered < 2)
            {
                computerInput =
                    computerInput * 10 + 4;

                computerDigitsEntered++;
            }

            return;
        }


        // -----------------------------------------------------
        // Number 5
        // -----------------------------------------------------

        if (key == GLFW_KEY_5)
        {
            if (computerDigitsEntered < 2)
            {
                computerInput =
                    computerInput * 10 + 5;

                computerDigitsEntered++;
            }

            return;
        }


        // -----------------------------------------------------
        // Number 6
        // -----------------------------------------------------

        if (key == GLFW_KEY_6)
        {
            if (computerDigitsEntered < 2)
            {
                computerInput =
                    computerInput * 10 + 6;

                computerDigitsEntered++;
            }

            return;
        }


        // -----------------------------------------------------
        // Number 7
        // -----------------------------------------------------

        if (key == GLFW_KEY_7)
        {
            if (computerDigitsEntered < 2)
            {
                computerInput =
                    computerInput * 10 + 7;

                computerDigitsEntered++;
            }

            return;
        }


        // -----------------------------------------------------
        // Number 8
        // -----------------------------------------------------

        if (key == GLFW_KEY_8)
        {
            if (computerDigitsEntered < 2)
            {
                computerInput =
                    computerInput * 10 + 8;

                computerDigitsEntered++;
            }

            return;
        }


        // -----------------------------------------------------
        // Number 9
        // -----------------------------------------------------

        if (key == GLFW_KEY_9)
        {
            if (computerDigitsEntered < 2)
            {
                computerInput =
                    computerInput * 10 + 9;

                computerDigitsEntered++;
            }

            return;
        }


        // -----------------------------------------------------
        // BACKSPACE
        // -----------------------------------------------------

        if (key == GLFW_KEY_BACKSPACE)
        {
            computerInput = 0;

            computerDigitsEntered = 0;

            computerError = false;

            return;
        }


        // -----------------------------------------------------
        // ENTER
        // -----------------------------------------------------

        if (key == GLFW_KEY_ENTER)
        {
            if (computerDigitsEntered == 2)
            {
                if (computerInput == 42)
                {
                    computerSolved = true;

                    computerOpen = false;

                    computerSolvedMessage = true;

                    computerError = false;
                }
                else
                {
                    computerError = true;

                    computerInput = 0;

                    computerDigitsEntered = 0;
                }
            }

            return;
        }


        // Do not allow normal movement while PC is open.
        return;
    }


    // ---------------------------------------------------------
    // NORMAL GAME INPUT
    // ---------------------------------------------------------

    if (key >= 0 &&
        key <= GLFW_KEY_LAST)
    {
        keyState[key] = true;
    }
}


// =============================================================
// KEY RELEASED
// =============================================================

void Game::keyReleased(int key)
{
    if (key >= 0 && key <= GLFW_KEY_LAST)
    {
        keyState[key] = false;
    }
}


// =============================================================
// UPDATE
// =============================================================

void Game::update(double deltaTime)
{
    // Prevent extremely large movement after a lag spike.
    if (deltaTime > 0.05)
    {
        deltaTime = 0.05;
    }


    // ---------------------------------------------------------
    // LOCKED DOOR OPENING ANIMATION
    // ---------------------------------------------------------

    if (doorOpening)
    {
        doorOpenAmount +=
            deltaTime / DOOR_OPEN_DURATION;

        if (doorOpenAmount >= 1.0)
        {
            doorOpenAmount = 1.0;

            doorOpening = false;

            doorOpen = true;
        }
    }


    // ---------------------------------------------------------
    // ROOM 1 -> ROOM 2 TRANSITION
    // ---------------------------------------------------------
    // Room 1 exits through its unlocked right-side door.
    // Room 2 receives the player through the clearly marked
    // doorway on its LEFT wall.
    if (currentRoom == 1 &&
        doorOpen &&
        playerX > Renderer::ROOM_RIGHT &&
        playerY >= 280.0 &&
        playerY <= 460.0)
    {
        currentRoom = 2;

        // Spawn just inside Room 2's left entrance.
        playerX = 105.0;
        playerY = 480.0;

        doorOpening = false;
        doorOpen = false;
        doorOpenAmount = 0.0;

        // Prevent a held E key from immediately interacting with
        // the first Room 2 puzzle.
        eWasPressed = keyState[GLFW_KEY_E];

        // First short Room 2 briefing.
        room2Message = true;
        room2MessageType = 1;
    }


    // ---------------------------------------------------------
    // ROOM 2 GENERATOR SCALING ANIMATION
    // ---------------------------------------------------------
    // Update this before the interaction/message early-return so the
    // generator can continue pulsing while its message is displayed.
    if (currentRoom == 2)
    {
        renderer->updateGeneratorAnimation(deltaTime);
    }


    // ---------------------------------------------------------
    // INTERACTION
    // ---------------------------------------------------------

    checkInteraction();
    

    // ---------------------------------------------------------
    // PC PUZZLE / ACCESS CARD MESSAGE
    //
    // Freeze the player while either interface is open.
    // ---------------------------------------------------------

    if (computerOpen ||
        computerSolvedMessage ||
        accessCardMessage ||
        accessGrantedMessage ||
        room2Message ||
        room2PuzzleOpen)
    {
        return;
    }


    // ---------------------------------------------------------
    // CCTV
    // ---------------------------------------------------------

    renderer->updateCCTV(deltaTime);


    // ---------------------------------------------------------
    // PLAYER MOVEMENT
    // ---------------------------------------------------------

    double deltaX = 0.0;
    double deltaY = 0.0;


    // Read GLFW directly as well as the callback state. This makes
    // movement reliable even if a key callback is missed while the
    // window is changing focus or the Room 1 -> Room 2 transition
    // occurs.
    const bool moveUp =
        keyState[GLFW_KEY_W] ||
        keyState[GLFW_KEY_UP] ||
        glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS;

    const bool moveDown =
        keyState[GLFW_KEY_S] ||
        keyState[GLFW_KEY_DOWN] ||
        glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS;

    const bool moveLeft =
        keyState[GLFW_KEY_A] ||
        keyState[GLFW_KEY_LEFT] ||
        glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS;

    const bool moveRight =
        keyState[GLFW_KEY_D] ||
        keyState[GLFW_KEY_RIGHT] ||
        glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS;

    if (moveUp)
        deltaY -= PLAYER_SPEED * deltaTime;

    if (moveDown)
        deltaY += PLAYER_SPEED * deltaTime;

    if (moveLeft)
        deltaX -= PLAYER_SPEED * deltaTime;

    if (moveRight)
        deltaX += PLAYER_SPEED * deltaTime;


    // ---------------------------------------------------------
    // NORMALISE DIAGONAL MOVEMENT
    // ---------------------------------------------------------

    if (deltaX != 0.0 && deltaY != 0.0)
    {
        constexpr double diagonalScale = 0.70710678118;

        deltaX *= diagonalScale;
        deltaY *= diagonalScale;
    }


    // ---------------------------------------------------------
    // MOVE X
    // ---------------------------------------------------------

    if (deltaX != 0.0)
    {
        const double newX = playerX + deltaX;

        if (!checkPlayerCollision(newX, playerY))
        {
            playerX = newX;
        }
    }


    // ---------------------------------------------------------
    // MOVE Y
    // ---------------------------------------------------------

    if (deltaY != 0.0)
    {
        const double newY = playerY + deltaY;

        if (!checkPlayerCollision(playerX, newY))
        {
            playerY = newY;
        }
    }


}


// =============================================================
// PLAYER COLLISION
// =============================================================

bool Game::checkPlayerCollision(
    double newX,
    double newY
) const
{
    const double radius = PLAYER_RADIUS;

    // =========================================================
    // ROOM 2 COLLISION
    // =========================================================
    if (currentRoom == 2)
    {
        // LEFT WALL HAS A REAL ENTRANCE OPENING.
        // This is the doorway through which Room 1 leads into
        // Room 2.
        const bool inRoom2Entrance =
            newY >= 430.0 &&
            newY <= 530.0;

        if (newY - radius < Renderer::ROOM_TOP)
            return true;

        if (newY + radius > Renderer::ROOM_BOTTOM)
            return true;

        if (newX + radius > Renderer::ROOM_RIGHT)
            return true;

        if (newX - radius < Renderer::ROOM_LEFT && !inRoom2Entrance)
            return true;

        // Fuse Box.
        if (checkRectangleCollision(newX, newY, radius,
                                    120.0, 85.0, 232.0, 218.0))
            return true;

        // Switch bank. It is kept above the entrance so the
        // player never spawns inside an obstacle.
        if (checkRectangleCollision(newX, newY, radius,
                                    55.0, 267.0, 142.0, 428.0))
            return true;

        // Generator.
        if (checkRectangleCollision(newX, newY, radius,
                                    445.0, 77.0, 662.0, 222.0))
            return true;

        // Electrical Panel.
        if (checkRectangleCollision(newX, newY, radius,
                                    881.0, 87.0, 982.0, 218.0))
            return true;

        // Workbench.
        if (checkRectangleCollision(newX, newY, radius,
                                    366.0, 323.0, 657.0, 404.0))
            return true;

        // Main switch.
        if (checkRectangleCollision(newX, newY, radius,
                                    791.0, 312.0, 867.0, 413.0))
            return true;

        // Storage crates.
        if (checkRectangleCollision(newX, newY, radius,
                                    1010.0, 447.0, 1065.0, 501.0))
            return true;

        if (checkRectangleCollision(newX, newY, radius,
                                    1065.0, 488.0, 1120.0, 540.0))
            return true;

        // Final exit door.
        if (!room2ExitDoorUnlocked &&
            checkRectangleCollision(newX, newY, radius,
                                    1040.0, 207.0, 1145.0, 403.0))
            return true;

        return false;
    }

    // =========================================================
    // ROOM 1 COLLISION
    // =========================================================

    if (newX - radius < Renderer::ROOM_LEFT)
        return true;

    if (newX + radius > Renderer::ROOM_RIGHT)
    {
        const bool atExitDoor =
            doorOpen &&
            newY >= 280.0 &&
            newY <= 460.0;

        if (!atExitDoor)
            return true;
    }

    if (newY - radius < Renderer::ROOM_TOP)
        return true;

    if (newY + radius > Renderer::ROOM_BOTTOM)
    {
        const bool insideEntry =
            newX >= Renderer::ENTRY_LEFT &&
            newX <= Renderer::ENTRY_RIGHT;

        if (!insideEntry)
            return true;
    }

    if (checkRectangleCollision(newX, newY, radius,
                                140.0, 300.0, 390.0, 435.0))
        return true;

    if (checkRectangleCollision(newX, newY, radius,
                                210.0, 295.0, 325.0, 370.0))
        return true;

    if (checkRectangleCollision(newX, newY, radius,
                                925.0, 240.0, 1040.0, 395.0))
        return true;

    if (checkRectangleCollision(newX, newY, radius,
                                1130.0, 555.0, 1210.0, 640.0))
        return true;

    if (checkRectangleCollision(newX, newY, radius,
                                555.0, 465.0, 785.0, 570.0))
        return true;

    if (checkRectangleCollision(newX, newY, radius,
                                72.0, 105.0, 138.0, 195.0))
        return true;

    if (!doorOpen)
    {
        if (checkRectangleCollision(newX, newY, radius,
                                    1105.0, 280.0, 1210.0, 460.0))
            return true;
    }

    return false;
}


// =============================================================
// CIRCLE VS RECTANGLE COLLISION
// =============================================================

bool Game::checkRectangleCollision(
    double playerX,
    double playerY,
    double playerRadius,
    double left,
    double top,
    double right,
    double bottom
) const
{
    // Find the closest point on the rectangle to the player.

    const double closestX =
        std::max(
            left,
            std::min(playerX, right)
        );

    const double closestY =
        std::max(
            top,
            std::min(playerY, bottom)
        );


    // Distance from player centre to closest point.

    const double dx = playerX - closestX;

    const double dy = playerY - closestY;


    const double distanceSquared =
        dx * dx +
        dy * dy;


    return distanceSquared <
           playerRadius * playerRadius;
}


// =============================================================
// PLAYER MOVEMENT HELPER
// =============================================================

void Game::movePlayer(
    double deltaX,
    double deltaY
)
{
    const double newX = playerX + deltaX;
    const double newY = playerY + deltaY;


    if (!checkPlayerCollision(newX, newY))
    {
        playerX = newX;
        playerY = newY;
    }
}


// =============================================================
// ROOM CHECK
// =============================================================

bool Game::insideRoom(
    double x,
    double y
) const
{
    const double radius = PLAYER_RADIUS;


    if (x - radius < Renderer::ROOM_LEFT)
    {
        return false;
    }

    if (x + radius > Renderer::ROOM_RIGHT)
    {
        return false;
    }

    if (y - radius < Renderer::ROOM_TOP)
    {
        return false;
    }

    if (y + radius > Renderer::ROOM_BOTTOM)
    {
        return false;
    }


    return true;
}


// =============================================================
// COMPUTER PROXIMITY
// =============================================================

// =============================================================
// COMPUTER INTERACTION AREA
// =============================================================

bool Game::isNearComputer() const
{
    // ---------------------------------------------------------
    // PC / RECEPTION DESK INTERACTION AREA
    // ---------------------------------------------------------
    //
    // The computer is sitting on the reception desk.
    //
    // The player approaches the computer from the FRONT of
    // the desk. The desk occupies approximately:
    //
    // X = 140 -> 390
    // Y = 300 -> 435
    //
    // Therefore the player interaction area is immediately
    // BELOW the desk.
    //
    // ---------------------------------------------------------

    const double interactionLeft   = 175.0;
    const double interactionRight  = 375.0;

    const double interactionTop    = 440.0;
    const double interactionBottom = 510.0;


    return
        playerX >= interactionLeft &&
        playerX <= interactionRight &&
        playerY >= interactionTop &&
        playerY <= interactionBottom;
}

// =============================================================
// CABINET INTERACTION AREA
// =============================================================

bool Game::isNearCabinet() const
{
    // ---------------------------------------------------------
    // CABINET INTERACTION AREA
    // ---------------------------------------------------------
    //
    // Cabinet itself:
    // X = 925 -> 1040
    // Y = 240 -> 395
    //
    // The player can interact from ANY side:
    //   - below
    //   - above
    //   - left
    //   - right
    //
    // We calculate the closest point on the cabinet and then
    // check the distance from the player to that point.
    // ---------------------------------------------------------

    const double cabinetLeft   = 925.0;
    const double cabinetRight  = 1040.0;
    const double cabinetTop    = 240.0;
    const double cabinetBottom = 395.0;

    const double interactionDistance = 55.0;

    double closestX = playerX;
    double closestY = playerY;

    // Clamp player position to the cabinet rectangle.
    if (closestX < cabinetLeft)
        closestX = cabinetLeft;

    if (closestX > cabinetRight)
        closestX = cabinetRight;

    if (closestY < cabinetTop)
        closestY = cabinetTop;

    if (closestY > cabinetBottom)
        closestY = cabinetBottom;

    const double dx = playerX - closestX;
    const double dy = playerY - closestY;

    const double distanceSquared =
        dx * dx + dy * dy;

    return distanceSquared <=
           interactionDistance * interactionDistance;
}

// =============================================================
// LOCKED DOOR INTERACTION AREA
// =============================================================

bool Game::isNearLockedDoor() const
{
    // ---------------------------------------------------------
    // LOCKED DOOR
    // ---------------------------------------------------------
    //
    // Door occupies approximately:
    //
    // X = 1105 -> 1210
    // Y = 280  -> 460
    //
    // The player can approach the door from the accessible
    // sides of the room.
    // ---------------------------------------------------------

    const double doorLeft   = 1105.0;
    const double doorRight  = 1210.0;
    const double doorTop    = 280.0;
    const double doorBottom = 460.0;

    const double interactionDistance = 55.0;


    // ---------------------------------------------------------
    // Find the closest point on the door rectangle
    // ---------------------------------------------------------

    double closestX = playerX;
    double closestY = playerY;


    if (closestX < doorLeft)
        closestX = doorLeft;

    if (closestX > doorRight)
        closestX = doorRight;

    if (closestY < doorTop)
        closestY = doorTop;

    if (closestY > doorBottom)
        closestY = doorBottom;


    // ---------------------------------------------------------
    // Distance from player to closest point
    // ---------------------------------------------------------

    const double dx = playerX - closestX;
    const double dy = playerY - closestY;

    const double distanceSquared =
        dx * dx + dy * dy;


    return distanceSquared <=
           interactionDistance * interactionDistance;
}

// =============================================================
// INTERACTION SYSTEM
// =============================================================

void Game::checkInteraction()
{
    const bool eIsPressed = keyState[GLFW_KEY_E];
    const bool eJustPressed = eIsPressed && !eWasPressed;

    // =========================================================
    // ROOM 2
    // =========================================================
    if (currentRoom == 2)
    {
        if (eJustPressed)
        {
            // Fuse Box
            if (isNearRoom2FuseBox())
            {
                if (!room2FuseInserted)
                {
                    room2FuseInserted = true;
                    room2Message = true;
                    room2MessageType = 2;
                }
                else
                {
                    room2Message = true;
                    room2MessageType = 2;
                }
            }

            // Workbench — actual cable-order puzzle.
            else if (isNearRoom2Workbench())
            {
                if (!room2FuseInserted)
                {
                    room2Message = true;
                    room2MessageType = 10;
                }
                else if (!room2PuzzleSolved)
                {
                    room2PuzzleOpen = true;
                    room2PuzzleStep = 0;
                    room2PuzzleInputCount = 0;
                    room2PuzzleError = false;
                    room2PuzzleInput[0] = '\0';
                    room2PuzzleInput[1] = '\0';
                    room2PuzzleInput[2] = '\0';
                    room2PuzzleInput[3] = '\0';
                    room2PuzzleInput[4] = '\0';
                }
                else
                {
                    room2Message = true;
                    room2MessageType = 9;
                }
            }

            // Cable System
            else if (isNearRoom2CableSystem())
            {
                if (!room2FuseInserted)
                {
                    room2Message = true;
                    room2MessageType = 10;
                }
                else if (!room2PuzzleSolved)
                {
                    room2Message = true;
                    room2MessageType = 11;
                }
                else if (!room2CablesConnected)
                {
                    room2CablesConnected = true;
                    room2Message = true;
                    room2MessageType = 3;
                }
            }

            // Generator
            else if (isNearRoom2Generator())
            {
                if (!room2CablesConnected)
                {
                    room2Message = true;
                    room2MessageType = 12;
                }
                else if (!room2GeneratorPowered)
                {
                    room2GeneratorPowered = true;
                    room2Message = true;
                    room2MessageType = 4;
                }
                else
                {
                    room2Message = true;
                    room2MessageType = 4;
                }
            }

            // Electrical Panel
            else if (isNearRoom2Panel())
            {
                if (!room2GeneratorPowered)
                {
                    room2Message = true;
                    room2MessageType = 13;
                }
                else if (!room2PanelActivated)
                {
                    room2PanelActivated = true;
                    room2Message = true;
                    room2MessageType = 5;
                }
            }

            // Main Switch
            else if (isNearRoom2MainSwitch())
            {
                if (!room2PanelActivated)
                {
                    room2Message = true;
                    room2MessageType = 14;
                }
                else if (!room2MainSwitchActivated)
                {
                    room2MainSwitchActivated = true;
                    room2ExitDoorUnlocked = true;
                    room2Message = true;
                    room2MessageType = 6;
                }
                else
                {
                    room2Message = true;
                    room2MessageType = 6;
                }
            }

            // Exit Door
            else if (isNearRoom2ExitDoor())
            {
                if (!room2ExitDoorUnlocked)
                {
                    room2Message = true;
                    room2MessageType = 7;
                }
                else if (!room2Finished)
                {
                    room2Finished = true;
                    room2Message = true;
                    room2MessageType = 8;
                }
            }
        }

        eWasPressed = eIsPressed;
        return;
    }

    // =========================================================
    // ROOM 1 — PC
    // =========================================================
    if (eJustPressed &&
        isNearComputer() &&
        !computerOpen &&
        !computerSolved)
    {
        computerInteracted = true;
        computerOpen = true;
        computerInput = 0;
        computerDigitsEntered = 0;
        computerError = false;
    }

    // =========================================================
    // ROOM 1 — CABINET
    // =========================================================
    if (eJustPressed &&
        computerSolved &&
        !accessCardObtained &&
        isNearCabinet())
    {
        accessCardObtained = true;
        accessCardMessage = true;
    }

    // =========================================================
    // ROOM 1 — LOCKED DOOR
    // =========================================================
    if (eJustPressed &&
        accessCardObtained &&
        !exitDoorUnlocked &&
        isNearLockedDoor())
    {
        exitDoorUnlocked = true;
        accessGrantedMessage = true;
    }

    eWasPressed = eIsPressed;
}



// =============================================================
// ROOM 2 — PROXIMITY HELPERS
// =============================================================

bool Game::isNearRoom2FuseBox() const
{
    return nearRectangle(playerX, playerY,
                         120.0, 85.0, 232.0, 218.0, 65.0);
}

bool Game::isNearRoom2Workbench() const
{
    return nearRectangle(playerX, playerY,
                         366.0, 323.0, 657.0, 404.0, 65.0);
}

bool Game::isNearRoom2CableSystem() const
{
    // Cable connection point between the Fuse Box and Generator.
    return nearRectangle(playerX, playerY,
                         230.0, 100.0, 445.0, 190.0, 60.0);
}

bool Game::isNearRoom2Generator() const
{
    return nearRectangle(playerX, playerY,
                         445.0, 77.0, 662.0, 222.0, 70.0);
}

bool Game::isNearRoom2Panel() const
{
    return nearRectangle(playerX, playerY,
                         881.0, 87.0, 982.0, 218.0, 65.0);
}

bool Game::isNearRoom2MainSwitch() const
{
    return nearRectangle(playerX, playerY,
                         791.0, 312.0, 867.0, 413.0, 65.0);
}

bool Game::isNearRoom2ExitDoor() const
{
    return nearRectangle(playerX, playerY,
                         1040.0, 207.0, 1145.0, 403.0, 65.0);
}

bool Game::nearRectangle(double px, double py,
                         double left, double top,
                         double right, double bottom,
                         double distance) const
{
    const double cx = std::max(left, std::min(px, right));
    const double cy = std::max(top, std::min(py, bottom));

    const double dx = px - cx;
    const double dy = py - cy;

    return dx * dx + dy * dy <= distance * distance;
}

// =============================================================
// ROOM 2 — DRAW/INTERACTION STATE
// =============================================================

// =============================================================
// RENDER
// =============================================================

void Game::render()
{
    renderer->beginFrame();


    // ---------------------------------------------------------
    // ROOM
    // ---------------------------------------------------------

   if (currentRoom == 1)
{
    renderer->drawRoom(
        exitDoorUnlocked,
        static_cast<float>(doorOpenAmount)
    );
}
else
{
    renderer->drawRoom2(
        room2FuseInserted,
        room2CablesConnected,
        room2GeneratorPowered,
        room2PanelActivated,
        room2MainSwitchActivated,
        room2ExitDoorUnlocked
    );
}


    // ---------------------------------------------------------
    // PLAYER
    // ---------------------------------------------------------

    renderer->drawPlayer(
        static_cast<float>(playerX),
        static_cast<float>(playerY)
    );


    // ---------------------------------------------------------
    // PC PUZZLE POPUP
    // ---------------------------------------------------------

    if (computerOpen)
    {
        renderer->drawComputerPuzzle(
            computerInput,
            computerDigitsEntered,
            computerError   
        );
    }


    // ---------------------------------------------------------
    // ACCESS CARD MESSAGE
    // ---------------------------------------------------------

    else if (accessCardMessage)
    {
        renderer->drawAccessCardMessage();
    }


    // ---------------------------------------------------------
    // COMPUTER SOLVED OBJECTIVE MESSAGE
    // ---------------------------------------------------------

    else if (computerSolvedMessage)
    {
        renderer->drawComputerSolvedMessage();
    }

    // ACCESS GRANTED MESSAGE
    else if (accessGrantedMessage)
    {
        renderer->drawAccessGrantedMessage();
    }


    // ---------------------------------------------------------
    // ROOM 2 MESSAGE / PUZZLE OVERLAY
    // ---------------------------------------------------------
    if (currentRoom == 2)
    {
        if (room2PuzzleOpen)
        {
            renderer->drawRoom2Puzzle(room2PuzzleInput, room2PuzzleInputCount, room2PuzzleError);
        }
        else if (room2Message)
        {
            renderer->drawRoom2Message(room2MessageType);
        }
    }

    // ---------------------------------------------------------
    // ROOM-SPECIFIC INTERACTION PROMPTS
    // ---------------------------------------------------------

    if (currentRoom == 1)
    {
        if (computerSolved &&
            !accessCardObtained &&
            isNearCabinet())
        {
            renderer->drawInteractionPrompt(980.0f, 410.0f);
        }
        else if (accessCardObtained &&
                 !exitDoorUnlocked &&
                 isNearLockedDoor())
        {
            renderer->drawInteractionPrompt(1120.0f, 500.0f);
        }
        else if (!computerSolved &&
                 isNearComputer())
        {
            renderer->drawInteractionPrompt(275.0f, 450.0f);
        }
    }
    else if (!room2Message && !room2PuzzleOpen)
    {
        if (isNearRoom2FuseBox())
            renderer->drawInteractionPrompt(175.0f, 245.0f);
        else if (isNearRoom2Workbench())
            renderer->drawInteractionPrompt(510.0f, 300.0f);
        else if (isNearRoom2CableSystem())
            renderer->drawInteractionPrompt(335.0f, 205.0f);
        else if (isNearRoom2Generator())
            renderer->drawInteractionPrompt(555.0f, 250.0f);
        else if (isNearRoom2Panel())
            renderer->drawInteractionPrompt(930.0f, 250.0f);
        else if (isNearRoom2MainSwitch())
            renderer->drawInteractionPrompt(830.0f, 300.0f);
        else if (isNearRoom2ExitDoor())
            renderer->drawInteractionPrompt(1090.0f, 440.0f);
    }

    renderer->endFrame();
}
