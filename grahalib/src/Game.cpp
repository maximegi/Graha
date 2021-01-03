#include <iostream>
#include <GL/glew.h>

#include "Game.hpp"

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

void Game::RenderLoop()
{
    musicAudio.play();
    int loop = 0;
	bool done = false;
    while(!done) 
    {
        // Event loop:
        SDL_Event e;
        while(mWindowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }
        while(loop == 0)
        {
            Bouton play(370., 215., 70., 20., 0.5, "Play");
            Bouton quit(370., 165., 70., 20., 0.5, "Quit");
            glClearColor(1.0,1.0,1.0,0.3);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            text.write(play.mtext, play.mx, play.my, play.mstext, glm::vec3(0., 0., 0.));
            text.write(quit.mtext, quit.mx, quit.my, quit.mstext, glm::vec3(0., 0., 0.));

            float currentFrame = mWindowManager.getTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            while(mWindowManager.pollEvent(e))
            {
                if(e.type == SDL_MOUSEBUTTONUP && e.button.x * (2./3.) >= play.mx && e.button.x * (2./3.) <= play.mx + play.mw
                    && (float(mWindowHeight) - e.button.y) * (2./3.) >= play.my && (float(mWindowHeight) - e.button.y) * (2./3.) <= play.my + play.mh)
                {
                    loop = 1;
                    selectAudio.play();
                }
                if(e.type == SDL_QUIT || mWindowManager.isKeyPressed(SDLK_ESCAPE) || (e.type == SDL_MOUSEBUTTONUP && e.button.x * (2./3.) >= quit.mx && e.button.x * (2./3.) <= quit.mx + quit.mw
                    && (float(mWindowHeight) - e.button.y) * (2./3.) >= quit.my && (float(mWindowHeight) - e.button.y) * (2./3.) <= quit.my + quit.mh))
                {
                    loop = 3;
                    done = true;
                }
            }
            if(mWindowManager.isKeyPressed(SDLK_p))
            {
                loop = 1;
                selectAudio.play();
            }
            if(mWindowManager.getMousePosition()[0] * (2./3.) >= play.mx && mWindowManager.getMousePosition()[0] * (2./3.) <= play.mx + play.mw
                && (float(mWindowHeight) - mWindowManager.getMousePosition()[1]) * (2./3.) >= play.my && (float(mWindowHeight) - mWindowManager.getMousePosition()[1]) * (2./3.) <= play.my + play.mh)
            {
                flyoverAudio.pause();
                flyoverAudio.play();
                text.write(play.mtext, play.mx, play.my, play.mstext, glm::vec3(1., 1., 0.5));
            }
            if(mWindowManager.getMousePosition()[0] * (2./3.) >= quit.mx && mWindowManager.getMousePosition()[0] * (2./3.) <= quit.mx + quit.mw
                && (float(mWindowHeight) - mWindowManager.getMousePosition()[1]) * (2./3.) >= quit.my && (float(mWindowHeight) - mWindowManager.getMousePosition()[1]) * (2./3.) <= quit.my + quit.mh)
            {
                flyoverAudio.pause();
                flyoverAudio.play();
                text.write(quit.mtext, quit.mx, quit.my, quit.mstext, glm::vec3(1., 1., 0.5));
            }
            mWindowManager.swapBuffers();
        }
        musicAudio.stop();
        musicAudio.play();
        while(loop == 1)
        {   
            while(mWindowManager.pollEvent(e))
            {
                if(e.type == SDL_QUIT || mWindowManager.isKeyPressed(SDLK_ESCAPE))
                {
                    loop = 3;
                    done = true;
                }
                if(mWindowManager.isKeyPressed(SDLK_f))
                {
                    loop = 2;
                }
            }
            glClearColor(0.1,0.2,0.4,0.3);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float currentFrame = mWindowManager.getTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            
        //EVENTS
            firstPlanet.processInput(mWindowManager,deltaTime, mMousePosition, footAudio);

        //MODELS
            firstPlanet.drawModels(mProjMatrix);

        //TEXTS
            text.write("Objectives", 0.1, 150.0, 0.7, glm::vec3(1., 1., 1.));

            mMousePosition = mWindowManager.getMousePosition();
            mWindowManager.swapBuffers();
        }
        musicAudio.stop();
        musicAudio.play();
        while(loop == 2)
        {
            Bouton replay(336., 215., 111., 20., 0.5, "Play again");
            Bouton quit(370., 165., 70., 20., 0.5, "Quit");
            glClearColor(1.0,1.0,1.0,0.3);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            text.write(replay.mtext, replay.mx, replay.my, replay.mstext, glm::vec3(0., 0., 0.));
            text.write(quit.mtext, quit.mx, quit.my, quit.mstext, glm::vec3(0., 0., 0.));

            float currentFrame = mWindowManager.getTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            while(mWindowManager.pollEvent(e))
            {
                if(e.type == SDL_MOUSEBUTTONUP && e.button.x * (2./3.) >= replay.mx && e.button.x * (2./3.) <= replay.mx + replay.mw
                    && (float(mWindowHeight) - e.button.y) * (2./3.) >= replay.my && (float(mWindowHeight) - e.button.y) * (2./3.) <= replay.my + replay.mh)
                {
                    loop = 1;
                    selectAudio.play();
                }
                if(e.type == SDL_QUIT || mWindowManager.isKeyPressed(SDLK_ESCAPE) || (e.type == SDL_MOUSEBUTTONUP && e.button.x * (2./3.) >= quit.mx && e.button.x * (2./3.) <= quit.mx + quit.mw
                    && (float(mWindowHeight) - e.button.y) * (2./3.) >= quit.my && (float(mWindowHeight) - e.button.y) * (2./3.) <= quit.my + quit.mh))
                {
                    loop = 3;
                    done = true;
                }
            }
            if(mWindowManager.isKeyPressed(SDLK_p))
            {
                loop = 1;
                selectAudio.play();
            }
            if(mWindowManager.getMousePosition()[0] * (2./3.) >= replay.mx && mWindowManager.getMousePosition()[0] * (2./3.) <= replay.mx + replay.mw
                && (float(mWindowHeight) - mWindowManager.getMousePosition()[1]) * (2./3.) >= replay.my && (float(mWindowHeight) - mWindowManager.getMousePosition()[1]) * (2./3.) <= replay.my + replay.mh)
            {
                flyoverAudio.pause();
                flyoverAudio.play();
                text.write(replay.mtext, replay.mx, replay.my, replay.mstext, glm::vec3(1., 1., 0.5));
            }
            if(mWindowManager.getMousePosition()[0] * (2./3.) >= quit.mx && mWindowManager.getMousePosition()[0] * (2./3.) <= quit.mx + quit.mw
                && (float(mWindowHeight) - mWindowManager.getMousePosition()[1]) * (2./3.) >= quit.my && (float(mWindowHeight) - mWindowManager.getMousePosition()[1]) * (2./3.) <= quit.my + quit.mh)
            {
                flyoverAudio.pause();
                flyoverAudio.play();
                text.write(quit.mtext, quit.mx, quit.my, quit.mstext, glm::vec3(1., 1., 0.5));
            }
            mWindowManager.swapBuffers();
        }
    }
}

void Game::close()
{
    //MODELS
	firstPlanet.deleteBuffers();

    //AUDIO
    musicAudio.deleteBuffer();
    footAudio.deleteBuffer();
    new_objectAudio.deleteBuffer();
    woodAudio.deleteBuffer();
    shutdownOpenAL();
}