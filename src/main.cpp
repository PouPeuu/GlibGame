#include <iostream>
#include <DissensionFramework/game.hpp>
#include <main_game_object.hpp>
#include <filesystem>
#include <DissensionFramework/resource.hpp>
#include <DissensionFramework/opengl_renderer.hpp>

using namespace DissensionFramework;
using namespace GlibGame;

int main() {
    Renderer* renderer = new OpenGLRenderer();
    Game game(800, 600, renderer, "Glib Game");

    MainGameObject* main_game_object = new MainGameObject();
    game.addGameObject(main_game_object);

    game.run();
}