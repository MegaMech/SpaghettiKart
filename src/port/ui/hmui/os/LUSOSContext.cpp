#include "LUSOSContext.h"
#include "libultraship.h"
#include "Engine.h"

void LUSOSContext::init() {}
void LUSOSContext::dispose() {}

Coord LUSOSContext::getMouseDelta() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Ship::Coords delta = wnd->GetMouseDelta();
    return Coord(static_cast<float>(delta.x), static_cast<float>(delta.y));
}

Coord LUSOSContext::getMousePosition() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Ship::Coords mouse = wnd->GetMousePos();
    return Coord(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
}

void LUSOSContext::setMousePosition(Coord& pos) {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Ship::Coords mousePos = { static_cast<int32_t>(pos.x), static_cast<int32_t>(pos.y) };
    wnd->SetMousePos(mousePos);
}

Coord LUSOSContext::getMouseWheel() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    Ship::CoordsF wheel = wnd->GetMouseWheel();
    return Coord(wheel.x * 4.0f, wheel.y * 4.0f);
}

bool LUSOSContext::isMouseButtonPressed(int button) {
    auto wnd = GameEngine::Instance->context->GetWindow();
    return wnd->GetMouseState(static_cast<Ship::MouseBtn>(button));
}

bool LUSOSContext::isMouseButtonReleased(int button) {
    auto wnd = GameEngine::Instance->context->GetWindow();
    return !wnd->GetMouseState(static_cast<Ship::MouseBtn>(button));
}

bool LUSOSContext::isMouseButtonDown(int button) {
    auto wnd = GameEngine::Instance->context->GetWindow();
    return wnd->GetMouseState(static_cast<Ship::MouseBtn>(button));
}

void LUSOSContext::setMouseCursor(int cursor) {
    // Not implemented
}

void LUSOSContext::setClipboardText(const char* text) {
    // Not implemented
}

bool LUSOSContext::isTouchDevice() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    // Not implemented
    return false;
}

bool LUSOSContext::isTouchActive() {
    auto wnd = GameEngine::Instance->context->GetWindow();
    // Not implemented
    return false;
}

const char* LUSOSContext::getClipboardText() {
    // Not implemented
    return "";
}

void LUSOSContext::showCursor(bool show) {
    auto wnd = GameEngine::Instance->context->GetWindow();
    wnd->SetCursorVisibility(show);
}
