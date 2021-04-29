//
// Created by root on 29.4.21..
//

#ifndef PROJECT_BASE_GRASS_H
#define PROJECT_BASE_GRASS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <learnopengl/shader.h>
#include <stb_image.h>
#include <rg/Error.h>

class Grass {
public:
    float vertices[20] = {
            // positions            // texture coords
             0.5f,  0.5f, 0.0f,     1.0f, 1.0f, // top right
             0.5f, -0.5f, 0.0f,     1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,     0.0f, 1.0f  // top left
    };
    unsigned int indices[6] = {
            0, 1, 3,
            1, 2, 3
    };

    unsigned int diffuseMap;
    unsigned int specularMap;
    unsigned int normalMap;

    float expandRatio = 1.0f;
    Shader grassShader;
    glm::mat4 model = glm::mat4(1.0f);
    unsigned int VAO, VBO, EBO;

    Grass(float expandRatio)
            : grassShader("resources/shaders/grass.vs", "resources/shaders/grass.fs") {

        if (expandRatio != 1.0)
            for (int i = 0; i < 20; i++)
                vertices[i] *= expandRatio;


        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);


        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);


        grassShader.setInt("material.diffuse", 0);
        diffuseMap = TextureFromFile("resources/textures/grass/Green-Grass-Ground-Texture-DIFFUSE.jpg");
        grassShader.use();
        //specularMap = TextureFromFile("resources/textures/grass/Green-Grass-Ground-Texture-SPECULAR.jpg");
        //normalMap = TextureFromFile("resources/textures/Green-Grass-Ground-Texture-NORMAL.jpg");



        //grassShader.setInt("material.specular", 1);
        //grassShader.setInt("material.normal", 2);

    }

    unsigned int TextureFromFile(const char *filename) {

        unsigned int textureID;
        glGenTextures(1, &textureID);

        int width, height, nrComponents;
        unsigned char *data = stbi_load(filename, &width, &height, &nrComponents, 0);
        if (data) {
            GLenum format;
            if (nrComponents == 1) {
                format = GL_RED;
            } else if (nrComponents == 3) {
                format = GL_RGB;
            } else if (nrComponents == 4) {
                format = GL_RGBA;
            }
            glBindTexture(GL_TEXTURE_2D, textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        } else {
            ASSERT(false, "Failed to load texture image");
        }
        stbi_image_free(data);
        return textureID;
    }

    void setup(glm::mat4 projection, glm::mat4 view, glm::vec3 viewPos)
    {
        grassShader.use();
        grassShader.setMat4("projection", projection);
        grassShader.setMat4("view", view);
        grassShader.setMat4("model", model);
        grassShader.setVec3("viewPos", viewPos);

        //TODO svetla
    }
    void draw()
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    }

};

#endif //PROJECT_BASE_GRASS_H
