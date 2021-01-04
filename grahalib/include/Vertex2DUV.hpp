#pragma once

#include <iostream>
#include <glm.hpp>

struct Vertex2DUV
{
    glm::vec2 position;
    glm::vec2 texture;

    Vertex2DUV(){};
    Vertex2DUV(glm::vec2 position, glm::vec2 texture):position(position), texture(texture){}
};
