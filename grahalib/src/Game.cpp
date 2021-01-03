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

struct Vertex2DUV
{
    glm::vec2 position;
    glm::vec2 texture;

    Vertex2DUV(){};
    Vertex2DUV(glm::vec2 position, glm::vec2 texture):position(position), texture(texture){}
};

void Game::RenderLoop(glimac::FilePath &applicationPath)
{
    //chargement textures
    std::unique_ptr<glimac::Image> imgBeginMenu = glimac::loadImage("assets/textures/begin.png");
    std::unique_ptr<glimac::Image> imgEndMenu = glimac::loadImage("assets/textures/end.png");
    if(imgBeginMenu == NULL || imgEndMenu == NULL)
    {
        std::cout << "Could not find texture" << std::endl;
    }
    glimac::Program program = glimac::loadProgram(applicationPath.dirPath() + "assets/shaders/text2D.vs.glsl", applicationPath.dirPath() + "assets/shaders/text2D.fs.glsl");
    GLint uTexture = glGetUniformLocation(program.getGLId(), "uTexture");
    GLuint vbo;
    GLuint vao;
    GLuint texture;
    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_TEXTURE = 1;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgBeginMenu->getWidth(), imgBeginMenu->getHeight(), 0, GL_RGBA, GL_FLOAT, imgBeginMenu->getPixels());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    Vertex2DUV vertices[] = {
    Vertex2DUV(glm::vec2(-1.0, -1.0), glm::vec2(0.0, 1.0)),
    Vertex2DUV(glm::vec2(1.0, -1.0), glm::vec2(1.0, 1.0)),
    Vertex2DUV(glm::vec2(1.0, 1.0), glm::vec2(1.0, 0.0)), 
    Vertex2DUV(glm::vec2(-1.0, 1.0), glm::vec2(0.0, 0.0))
    };
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    
    glVertexAttribPointer(VERTEX_ATTR_POSITION,2,GL_FLOAT,GL_FALSE,sizeof(Vertex2DUV),(const GLvoid*)(offsetof(Vertex2DUV, position)));
    glVertexAttribPointer(VERTEX_ATTR_TEXTURE,2,GL_FLOAT,GL_FALSE,sizeof(Vertex2DUV),(const GLvoid*)(offsetof(Vertex2DUV, texture)));
    
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

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
            Bouton play(385., 265., 70., 20., 0.5, "Play");
            Bouton quit(385., 225., 70., 20., 0.5, "Quit");
            glClear(GL_COLOR_BUFFER_BIT);
            program.use();
            glBindVertexArray(vao);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(uTexture, 0);
            glDrawArrays(GL_QUADS,0,4);
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
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
        glDeleteBuffers(1,&vbo);
        glDeleteVertexArrays(1,&vao);
        glDeleteTextures(1, &texture);
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
            firstPlanet.quest(text, mWindowManager);
        //MODELS
            firstPlanet.drawModels(mProjMatrix);

        //TEXTS
            text.write("Objectives", 0.1, 150.0, 0.7, glm::vec3(1., 1., 1.));

            mMousePosition = mWindowManager.getMousePosition();
            mWindowManager.swapBuffers();
        }
        musicAudio.stop();
        musicAudio.play();

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgEndMenu->getWidth(), imgEndMenu->getHeight(), 0, GL_RGBA, GL_FLOAT, imgEndMenu->getPixels());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        Vertex2DUV vertices[] = {
        Vertex2DUV(glm::vec2(-1.0, -1.0), glm::vec2(0.0, 1.0)),
        Vertex2DUV(glm::vec2(1.0, -1.0), glm::vec2(1.0, 1.0)),
        Vertex2DUV(glm::vec2(1.0, 1.0), glm::vec2(1.0, 0.0)), 
        Vertex2DUV(glm::vec2(-1.0, 1.0), glm::vec2(0.0, 0.0))
        };
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Vertex2DUV), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);

        glGenVertexArrays(1,&vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
        glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        glVertexAttribPointer(VERTEX_ATTR_POSITION,2,GL_FLOAT,GL_FALSE,sizeof(Vertex2DUV),(const GLvoid*)(offsetof(Vertex2DUV, position)));
        glVertexAttribPointer(VERTEX_ATTR_TEXTURE,2,GL_FLOAT,GL_FALSE,sizeof(Vertex2DUV),(const GLvoid*)(offsetof(Vertex2DUV, texture)));

        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
        while(loop == 2)
        {
            Bouton replay(353., 265., 111., 20., 0.5, "Play again");
            Bouton quit(385., 225., 70., 20., 0.5, "Quit");
            glClearColor(1.0,1.0,1.0,0.3);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClear(GL_COLOR_BUFFER_BIT);
            program.use();
            glBindVertexArray(vao);
            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(uTexture, 0);
            glDrawArrays(GL_QUADS,0,4);
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
            text.write(replay.mtext, replay.mx, replay.my, replay.mstext, glm::vec3(1., 1., 1.));
            text.write(quit.mtext, quit.mx, quit.my, quit.mstext, glm::vec3(1., 1., 1.));

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