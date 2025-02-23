#include <main_game_object.hpp>
#include <iostream>

using namespace DissensionFramework;

void GlibGame::MainGameObject::_connectedToGame() {
    printf("MainGameObject connected to game\n");
}

void GlibGame::MainGameObject::update() {
    _state.should_redraw = true;
    offset += 0.1;
}

GlibGame::MainGameObject::MainGameObject() {
    printf("MainGameObject constructed\n");
    offset = 0;
    
}

void GlibGame::MainGameObject::draw(const Renderer* renderer) const {
    renderer->drawEquilateralPolygon(1293058, 0.5f, Color(1.0, 0.0, sin(offset) / 2 + 0.5, 1.0));
}