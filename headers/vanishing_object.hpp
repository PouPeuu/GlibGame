#ifndef VANISHING_OBJECT_H
#define VANISHING_OBJECT_H

#include <DissensionFramework/game_object.hpp>

using namespace DissensionFramework;

namespace GlibGame {
    class VanishingObject : public GameObject {
        private:
            unsigned int _life;
        public:
            VanishingObject(unsigned int life);
            State update();
    };
}

#endif