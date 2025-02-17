#include <main_game_object.hpp>
#include <vanishing_object.hpp>
#include <iostream>

void GlibGame::MainGameObject::_createNewVanishingObject() {
    _vanishing_object = new VanishingObject(10);
    _game->addGameObject(_vanishing_object);
    _vanishing_object->getDeletionSignal().subscribe([this]() {this->_createNewVanishingObject();});
}

void GlibGame::MainGameObject::_connectedToGame() {
    printf("MainGameObject connected to game\n");
    _createNewVanishingObject();
}

State GlibGame::MainGameObject::update() {
    

    return NORMAL;
}

GlibGame::MainGameObject::MainGameObject() {
    printf("MainGameObject constructed\n");
}