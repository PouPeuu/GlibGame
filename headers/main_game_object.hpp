#ifndef MAIN_GAME_OBJECT_HPP
#define MAIN_GAME_OBJECT_HPP

#include <DissensionFramework/game_object.hpp>
#include <vanishing_object.hpp>

using namespace DissensionFramework;

namespace GlibGame {
    class MainGameObject : public GameObject {
        protected:
            VanishingObject* _vanishing_object;

            void _connectedToGame();
            void _vanishingObjectVanished();
            void _createNewVanishingObject();
        public:
            MainGameObject();

            State update();
    };
}

#endif