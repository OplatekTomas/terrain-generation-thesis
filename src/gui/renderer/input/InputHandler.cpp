#include <renderer/input/InputHandler.h>

using namespace MapGenerator::Renderer;

InputHandler::InputHandler() {
    for(int i = 0; i < 512; i++) {
        keys[i] = false;
    }

    for(int i = 0; i < 3; i++) {
        mouseButtons[i] = false;
    }
}

InputHandler::~InputHandler() {
}

void InputHandler::mouseMove(int x, int y){
    //TODO implement
}

void InputHandler::mousePress(MouseButton button){
    mouseButtons[button] = true;
}

void InputHandler::mouseRelease(MouseButton button){
    mouseButtons[button] = false;
}

void InputHandler::mouseWheel(int wheel, int direction, int x, int y){
    //todo implement
}

void InputHandler::keyPress(Key key){
    keys[key] = true;
}

void InputHandler::keyRelease(Key key){
    keys[key] = false;
}

bool InputHandler::isKeyPressed(Key key){
    return keys[key];
}

bool InputHandler::isKeyUp(Key key){
    return !keys[key];
}

bool InputHandler::isMouseButtonPressed(MouseButton button){
    return mouseButtons[button];
}

bool InputHandler::isMouseButtonUp(MouseButton button){
    return !mouseButtons[button];
}

std::tuple<int, int> InputHandler::getPositionDelta(){
    return std::make_tuple(mouseX, mouseY);
}


