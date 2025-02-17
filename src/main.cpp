#include <iostream>
#include <DissensionFramework/game.hpp>
#include <main_game_object.hpp>

using namespace DissensionFramework;
using namespace GlibGame;

int main() {
    printf("Hello from main\n");
    Game game(800, 600, "Glib Game");
    printf("game created\n");

    MainGameObject* main_game_object = new MainGameObject();
    game.addGameObject(main_game_object);

    game.run();
}

// TODO: WON'T COMPIlE