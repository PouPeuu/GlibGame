#ifndef MAIN_GAME_OBJECT_HPP
#define MAIN_GAME_OBJECT_HPP

#include <DissensionFramework/game_object.hpp>

namespace GlibGame {
    class MainGameObject : public DissensionFramework::GameObject {
        protected:
            float offset;
            void _connectedToGame();
        public:
            MainGameObject();

            void update() override;
            void draw(const DissensionFramework::Renderer* renderer) const override;
    };
}

#endif