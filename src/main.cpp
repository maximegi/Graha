#include <SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <map>


//texte
#include <ft2build.h>
#include FT_FREETYPE_H

//glimac
#include <common.hpp>
#include <FilePath.hpp>
#include <Geometry.hpp>
#include <Program.hpp>
#include <Sphere.hpp>

//glm
#include <glm.hpp>

//modellib
#include <Model.hpp>


#include "FreeflyCamera.hpp"

using namespace glimac;

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

std::map<char, Character> Characters;

struct TextProgram {
    Program m_Program;
    GLuint locationProjMatrix;
    GLuint locationText;
    GLuint locationTextColor;

    TextProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "assets/shaders/text.vs.glsl",
                              applicationPath.dirPath() + "assets/shaders/text.fs.glsl")) {
        locationProjMatrix = glGetUniformLocation(m_Program.getGLId(), "uProjection");
        locationText = glGetUniformLocation(m_Program.getGLId(), "uText");
        locationTextColor = glGetUniformLocation(m_Program.getGLId(), "uTextColor");
    }

};

struct ModelProgram {
    Program m_Program;

    GLuint locationMVPMatrix;
    GLuint locationMVMatrix;
    GLuint locationModelMatrix;
    GLuint locationNormalMatrix;
    GLuint locationLightDirection;
    GLuint locationLightIntensity;

    ModelProgram(const FilePath& applicationPath):
        m_Program(loadProgram(applicationPath.dirPath() + "assets/shaders/3D.vs.glsl",
                              applicationPath.dirPath() + "assets/shaders/directionallighttextures.fs.glsl")) {
        locationMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
        locationMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
        locationModelMatrix = glGetUniformLocation(m_Program.getGLId(), "uModelMatrix");
        locationNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
        locationLightDirection = glGetUniformLocation(m_Program.getGLId(), "uLightDirection");
        locationLightIntensity = glGetUniformLocation(m_Program.getGLId(), "uLightIntensity");
    }
};

void RenderText(std::string text, float x, float y, float scale, GLuint colorLocation, glm::vec3 color, GLuint VAO, GLuint VBO)
{
    // activate corresponding render state  
    glUniform3fv(colorLocation, 1, glm::value_ptr(color));
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    const unsigned int WINDOW_W = 1200;
    const unsigned int WINDOW_H = 900;

    SDLWindowManager windowManager(WINDOW_W, WINDOW_H, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
     

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    FilePath applicationPath(argv[0]);
    ModelProgram modelProgram(applicationPath);
    TextProgram textProgram(applicationPath);
   
    //text
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "assets/fonts/roboto/Roboto-Regular.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;  
        return -1;
    }
    else {
        FT_Set_Pixel_Sizes(face, 0, 48); 
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction
  
        for (unsigned char c = 0; c < 128; c++)
        {
            // load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture, 
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                face->glyph->advance.x
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
    }

    // destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
    //endtext

    //INITIALIZATION
    std::string cubeFile = "assets/models/cube/cube_textures.fbx";
    Model cube(cubeFile);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //MATRIXES
    glm::mat4 ProjMatrix = glm::perspective(glm::radians(70.f),((float)WINDOW_W)/((float)WINDOW_H),0.1f,100.f);
    glm::mat4 ModelMatrix;
    glm::mat4 MVMatrix;
    glm::mat4 NormalMatrix;

    glm::mat4 textProjection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

    //CAMERA
    FreeflyCamera camera;
    glm::vec2 mousePosition = windowManager.getMousePosition();


    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        //RENDERING CODE
        glClearColor(0.1,0.2,0.4,0.3);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//
        
        //EVENTS
        if(windowManager.isMouseButtonPressed(SDL_BUTTON_RIGHT))
        {
            camera.rotateLeft((mousePosition[0] - windowManager.getMousePosition()[0])/10.f);
            camera.rotateUp((mousePosition[1] - windowManager.getMousePosition()[1])/10.f);
        }

        if(windowManager.isKeyPressed(SDLK_q))
        {
            camera.moveLeft(0.1f);
        }
        if(windowManager.isKeyPressed(SDLK_z))
        {
            camera.moveFront(0.1f);
        }
        if(windowManager.isKeyPressed(SDLK_d))
        {
            camera.moveLeft(-0.1f);
        }
        if(windowManager.isKeyPressed(SDLK_s))
        {
            camera.moveFront(-0.1f);
        }
        if(windowManager.isKeyPressed(SDLK_SPACE))
        {
            camera.moveUp(0.1f);
        }

        if (windowManager.isKeyPressed(SDLK_i)){
            glDisable(GL_BLEND);
            glEnable(GL_DEPTH_TEST);
        }
        else{
            glDisable(GL_DEPTH_TEST);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
        }



    //MODELS
        modelProgram.m_Program.use();

        //LIGHTS
        glUniform3fv(modelProgram.locationLightDirection,1,glm::value_ptr(glm::vec3(-1.,-1.,0.))); //Position of the light, don't forget to multiply by the view matrix
        glUniform3fv(modelProgram.locationLightIntensity,1,glm::value_ptr(glm::vec3(1.,1.,1.))); //Color of the light

    //VOLCANO
        //MATRIXES
        ModelMatrix = glm::translate(glm::mat4(1),glm::vec3(-2.,-2.,-5.));
        MVMatrix = camera.getViewMatrix()*ModelMatrix;
        NormalMatrix = glm::transpose(glm::inverse(ModelMatrix));


        glUniformMatrix4fv(modelProgram.locationModelMatrix,1,GL_FALSE,glm::value_ptr(ModelMatrix));
        glUniformMatrix4fv(modelProgram.locationMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(modelProgram.locationMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix*MVMatrix));
        glUniformMatrix4fv(modelProgram.locationNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));
        
        //COLORS
        // glUniform3fv(glGetUniformLocation(program.getGLId(),"uKa"),1,glm::value_ptr(glm::vec3(0.05375,0.05,0.06625)));
        // glUniform3fv(glGetUniformLocation(program.getGLId(),"uKd"),1,glm::value_ptr(glm::vec3(0.18275,0.17,0.22525)));
        // glUniform3fv(glGetUniformLocation(program.getGLId(),"uKs"),1,glm::value_ptr(glm::vec3(0.332741,0.328634,0.346435)));
        glUniform1f(glGetUniformLocation(modelProgram.m_Program.getGLId(), "uShininess"),0.3*128.f);

        //DRAW
        cube.DrawColors();

    //LAVA
        //MATRIXES
        ModelMatrix = glm::translate(glm::mat4(1),glm::vec3(2.,-2.,-5.));
        MVMatrix = camera.getViewMatrix()*ModelMatrix;
        NormalMatrix = glm::transpose(glm::inverse(ModelMatrix));

        glUniformMatrix4fv(modelProgram.locationModelMatrix,1,GL_FALSE,glm::value_ptr(ModelMatrix));
        glUniformMatrix4fv(modelProgram.locationMVMatrix,1,GL_FALSE,glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(modelProgram.locationMVPMatrix,1,GL_FALSE,glm::value_ptr(ProjMatrix*MVMatrix));
        glUniformMatrix4fv(modelProgram.locationNormalMatrix,1,GL_FALSE,glm::value_ptr(NormalMatrix));

        //COLORS
        // glUniform3fv(glGetUniformLocation(program.getGLId(),"uKa"),1,glm::value_ptr(glm::vec3(0.19125,0.0735,0.0225)));
        // glUniform3fv(glGetUniformLocation(program.getGLId(),"uKd"),1,glm::value_ptr(glm::vec3(0.7038,0.27048,0.0828)));
        // glUniform3fv(glGetUniformLocation(program.getGLId(),"uKs"),1,glm::value_ptr(glm::vec3(0.256777,0.137622,0.086014)));
        glUniform1f(glGetUniformLocation(modelProgram.m_Program.getGLId(), "uShininess"),0.1*128.f);

        //DRAW
        cube.DrawColors();
        glDisable(GL_DEPTH_TEST);

    //TEXTS
        textProgram.m_Program.use();
        glUniformMatrix4fv(textProgram.locationProjMatrix, 1, GL_FALSE, glm::value_ptr(textProjection));
        RenderText("coucou sa va", 0.0, 0.0, 1.0, textProgram.locationTextColor ,glm::vec3(1.0, 0.0, 0.2), vao, vbo);

        

        //END OF RENDERING CODE

        mousePosition = windowManager.getMousePosition();

        // Update the display
        windowManager.swapBuffers();
    }
    cube.deleteBuffers();

    return EXIT_SUCCESS;
}

