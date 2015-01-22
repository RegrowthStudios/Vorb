#include "stdafx.h"
#include "ui/ScreenList.h"

#include "ui/IGameScreen.h"

vui::ScreenList::ScreenList(MainGame* g) {
    _game = g;
    _current = SCREEN_INDEX_NO_START_SELECTED;
}

vui::IGameScreen* vui::ScreenList::moveNext() {
    const IGameScreen* cur = getCurrent();
    _current = cur == 0 ? SCREEN_INDEX_NO_SCREEN : cur->getNextScreen();
    return getCurrent();
}
vui::IGameScreen* vui::ScreenList::movePrevious() {
    const IGameScreen* cur = getCurrent();
    _current = cur == 0 ? SCREEN_INDEX_NO_SCREEN : cur->getPreviousScreen();
    return getCurrent();
}

void vui::ScreenList::setScreen(i32 s) {
    _current = s;
}
vui::ScreenList* vui::ScreenList::addScreens(IGameScreen** s, i32 c /*= 0*/) {
    // Check For Arguments
    if (s == 0 || c < 1) return this;

    // Copy Over The Screens
    int cs = _screens.size(), ds = cs + c;
    _screens.resize(ds);
    for (int i = 0; i < c; i++) {
        _screens[cs + i] = s[i];
    }

    // Build The Added Screens
    for (int i = cs; i < ds; i++) {
        if (_screens[i] == 0) continue;
        _screens[i]->setParentGame(_game, i);
        _screens[i]->build();
    }

    return this;
}
vui::ScreenList* vui::ScreenList::addScreen(IGameScreen* s) {
    _screens.push_back(s);
    if (s != 0) {
        s->setParentGame(_game, _screens.size() - 1);
        s->build();
    }
    return this;
}

void vui::ScreenList::destroy(GameTime gameTime) {
    int s = _screens.size();
    if (s < 1) return;
    for (int i = 0; i < s; i++) {
        _screens[i]->destroy(gameTime);
    }
    _screens.resize(0);
}
