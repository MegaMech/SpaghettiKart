#pragma once

#include "hmui/src/hmui/os/OSContext.h"

class LUSOSContext : public OSContext {
public:
    void init() override;
    void dispose() override;
    Coord getMouseDelta() override;
    Coord getMousePosition() override;
    void setMousePosition(Coord& pos) override;
    Coord getMouseWheel() override;
    bool isMouseButtonPressed(int button) override;
    bool isMouseButtonReleased(int button) override;
    bool isMouseButtonDown(int button) override;
    void setMouseCursor(int cursor) override;
    bool isTouchDevice() override;
    bool isTouchActive() override;
    void setClipboardText(const char* text) override;
    const char* getClipboardText() override;
    void showCursor(bool show) override;
    ~LUSOSContext() override = default;
};
