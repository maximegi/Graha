#include <SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>

//texte
#include <ft2build.h>
#include FT_FREETYPE_H

//glimac
#include <FilePath.hpp>
#include <Program.hpp>

//glm
#include <glm.hpp>

//grahalib
#include <Game.hpp>

#include "Text.hpp"

using namespace glimac;

int main(int argc, char** argv) {

    const unsigned int WINDOW_W = 1200;
    const unsigned int WINDOW_H = 900;

    // Initialize SDL

    SDLWindowManager window(WINDOW_W, WINDOW_H, "Graha");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    //GAME

    //Initialization
    FilePath applicationPath(argv[0]);
    Game graha(WINDOW_W, WINDOW_H, window, applicationPath);

    // Application loop:
    graha.RenderLoop();

    //Delete buffers from planets
    graha.close();

    return EXIT_SUCCESS;
}