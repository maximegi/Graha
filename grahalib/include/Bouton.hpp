#pragma once

#include <iostream>
#include <glm.hpp>

struct Bouton
{
    float mx;
    float my;
    float mw;
    float mh;
    float mstext;
    std::string mtext;
    Bouton(float x, float y, float w, float h, float stext, std::string text)
    {
        mx = x;
        my = y;
        mw = w;
        mh = h;
        mstext = stext;
        mtext = text;
    }
};