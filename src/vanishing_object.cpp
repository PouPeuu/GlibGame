#include <vanishing_object.hpp>
#include <iostream>

GlibGame::VanishingObject::VanishingObject(unsigned int life) {
    printf("VanishingObject created with life %u\n", life);
    _life = life;
}

State GlibGame::VanishingObject::update() {
    _life -= 1;
    std::cout << _life << " life remaining" << std::endl;
    if (_life <= 0) {
        printf("deleting VanishingObject\n");
        return QUEUE_DELETE;
    }
    return NORMAL;
}