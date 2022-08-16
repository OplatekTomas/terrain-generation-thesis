#pragma once

#include <tuple>
namespace MapGenerator::Renderer{
    class InputHandler {
    public:
        InputHandler();
        ~InputHandler();

        enum Key {
            KEY_UNKNOWN = 0,
            KEY_SPACE = 32,
            KEY_APOSTROPHE = 39,
            KEY_COMMA = 44,
            KEY_MINUS = 45,
            KEY_PERIOD = 46,
            KEY_SLASH = 47,
            KEY_0 = 48,
            KEY_1 = 49,
            KEY_2 = 50,
            KEY_3 = 51,
            KEY_4 = 52,
            KEY_5 = 53,
            KEY_6 = 54,
            KEY_7 = 55,
            KEY_8 = 56,
            KEY_9 = 57,
            KEY_SEMICOLON = 59,
            KEY_EQUAL = 61,
            KEY_A = 65,
            KEY_B = 66,
            KEY_C = 67,
            KEY_D = 68,
            KEY_E = 69,
            KEY_F = 70,
            KEY_G = 71,
            KEY_H = 72,
            KEY_I = 73,
            KEY_J = 74,
            KEY_K = 75,
            KEY_L = 76,
            KEY_M = 77,
            KEY_N = 78,
            KEY_O = 79,
            KEY_P = 80,
            KEY_Q = 81,
            KEY_R = 82,
            KEY_S = 83,
            KEY_T = 84,
            KEY_U = 85,
            KEY_V = 86,
            KEY_W = 87,
            KEY_X = 88,
            KEY_Y = 89,
            KEY_Z = 90,
            KEY_LEFT_BRACKET = 91,
            KEY_BACKSLASH = 92,
            KEY_RIGHT_BRACKET = 93,
            KEY_GRAVE_ACCENT = 96,
            KEY_WORLD_1 = 161,
            KEY_WORLD_2 = 162,
            KEY_ESCAPE = 256,
            KEY_ENTER = 257,
            KEY_TAB = 258,
            KEY_BACKSPACE = 259,
            KEY_INSERT = 260,
        };

        enum MouseButton {
            MOUSE_BUTTON_UNKNOWN = 0,
            MOUSE_LEFT = 1,
            MOUSE_RIGHT = 2,
            MOUSE_MIDDLE = 4,
        };

        void mouseMove(int x, int y);
        void mousePress(MouseButton button, int x, int y);
        void mouseRelease(MouseButton button, int x, int y);
        void mouseWheel(int wheel, int direction, int x, int y);
        void keyPress(Key key);
        void keyRelease(Key key);

        bool isKeyPressed(Key key);
        bool isKeyUp(Key key);
        bool isMouseButtonPressed(MouseButton button);
        bool isMouseButtonUp(MouseButton button);
        std::tuple<int, int> getPositionDelta();
        std::tuple<int, int> getAbsolutePosition();


    private:

        bool keys[512];
        bool mouseButtons[3];
        int deltaX = 0;
        int deltaY = 0;
        int absoluteX = 0;
        int absoluteY = 0;
    };
}


