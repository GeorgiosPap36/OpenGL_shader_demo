#ifndef GENERATE_PRIMITIVES_H
#define GENERATE_PRIMITIVES_H

#include "../model.h"


class GeneratePrimitives {

    Model generateCube(glm::vec3 size, std::vector<Texture> textures);
};

#endif