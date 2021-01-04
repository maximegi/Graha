#include <iostream>
#include <GL/glew.h>
#include "Menu.hpp"
#include "Bouton.hpp"

#include "Game.hpp"

void Game::RenderLoop(glimac::FilePath &applicationPath)
{
    Menu beginMenu("assets/textures/begin.png", applicationPath);

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

            Bouton play(385., 265., 50., 20., 0.5, "Play");
            Bouton quit(385., 225., 50., 20., 0.5, "Quit");

            glClear(GL_COLOR_BUFFER_BIT);

            beginMenu.draw();

            text.write(play.mtext, play.mx, play.my, play.mstext, glm::vec3(1., 1., 1.));
            text.write(quit.mtext, quit.mx, quit.my, quit.mstext, glm::vec3(1., 1., 1.));

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
        beginMenu.deleteBuffers();

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
            }
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            float currentFrame = mWindowManager.getTime();
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;
            
        //EVENTS
            firstPlanet.processInput(mWindowManager,deltaTime, mMousePosition, footAudio);

        //SKY
            firstPlanet.drawSky(mProjMatrix);
        //MODELS
            firstPlanet.drawModels(mProjMatrix);

        //TEXTS
            firstPlanet.quest(text, mWindowManager, woodAudio, new_objectAudio, roseAudio, blueAudio, yellowAudio, applauseAudio);
            if(firstPlanet.getmEnd())
            {
                loop = 2;
            }
            mMousePosition = mWindowManager.getMousePosition();
            mWindowManager.swapBuffers();
        }
        musicAudio.stop();
        musicAudio.play();

        
        Menu endMenu("assets/textures/end.png", applicationPath);

        while(loop == 2)
        {
            Bouton quit(385., 225., 50., 20., 0.5, "Quit");

            glClear(GL_COLOR_BUFFER_BIT);

            endMenu.draw();
            
            text.write("C'est la fin du jeu je te jure, t'es triste non ?", 190., 365., 0.5, glm::vec3(1., 1., 1.));
            text.write(quit.mtext, quit.mx, quit.my, quit.mstext, glm::vec3(1., 1., 1.));

            while(mWindowManager.pollEvent(e))
            {
                if(e.type == SDL_QUIT || mWindowManager.isKeyPressed(SDLK_ESCAPE) || (e.type == SDL_MOUSEBUTTONUP && e.button.x * (2./3.) >= quit.mx && e.button.x * (2./3.) <= quit.mx + quit.mw
                    && (float(mWindowHeight) - e.button.y) * (2./3.) >= quit.my && (float(mWindowHeight) - e.button.y) * (2./3.) <= quit.my + quit.mh))
                {
                    loop = 3;
                    done = true;
                }
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

        endMenu.deleteBuffers();
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
    roseAudio.deleteBuffer();
    blueAudio.deleteBuffer();
    yellowAudio.deleteBuffer();
    applauseAudio.deleteBuffer();
    shutdownOpenAL();
}