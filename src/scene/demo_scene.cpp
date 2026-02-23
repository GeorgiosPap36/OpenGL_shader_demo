#ifndef DEMO_SCENE_H
#define DEMO_SCENE_H

#include "scene.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "../model/material.h"

#include "../camera/basic_camera_controller.cpp"

class DemoScene : public Scene {

    private:
    unsigned int SCR_WIDTH, SCR_HEIGHT;
    BasicCameraController camera;

    std::map<Material*, std::vector<Model*>> renderBatches;

    glm::mat4 projectionMat;
    glm::vec3 directionalLight;

    

    public:
    DemoScene(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT) : 
                SCR_WIDTH(SCR_WIDTH), SCR_HEIGHT(SCR_HEIGHT),
                camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -1), glm::vec3(0), 0, -90, 75)
    {
        
        setUpScene();

        setUpRenderBatches();

        setUpFBOs();

        std::cout << "Loaded: 'demo_scene'!" << std::endl;
    }

    void render(float interPolation) override  {
        generateDepthMap();
 
        glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (const auto& [material, renderBatch] : renderBatches) {
            Shader* matShader = material->shader;
            matShader->use();

            matShader->set("view", camera.viewMatrix());

            for (const auto& [name, value] : material->uniforms) {
                std::visit([&](auto&& v) {
                    matShader->set(name.c_str(), v);
                }, value);
            }

            for (const auto& model : renderBatch) {
                matShader->set("model", model->modelMatrix());
                model->draw(*matShader);
            }
        }
        
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Map the above render to a texture
        // Run a post processing shader

        renderQuad(*renderShader, renderTexture);
    }

    void update(float dt, std::map<int, bool>& keyboard, glm::vec2 mouseMovement) override {
        camera.update(dt, keyboard, mouseMovement);

        
    }

    private:
    float nearPlane = 0.1, farPlane = 200.0;
    // depthMap
    Shader* depthShader = new Shader("../src/shaders/vertex_shaders/depth_shader.vs", "../src/shaders/fragment_shaders/depth_shader.fs");
    unsigned int depthMapFBO, depthMap;
    // renderTexture
    Shader* renderShader = new Shader("../src/shaders/vertex_shaders/render_shader.vs", "../src/shaders/fragment_shaders/render_shader.fs");
    unsigned int renderFBO, renderDepthRBO, renderTexture;

    void setUpScene() {
        projectionMat = glm::perspective(camera.fovY, (float) SCR_WIDTH / (float) SCR_HEIGHT, nearPlane, farPlane);

        directionalLight = glm::vec3(0.0, -0.25, -0.75);

        SceneNode testNode;

        testNode.model = std::make_unique<Model>(std::filesystem::path("../assets/models/guitar_backpack/backpack.obj").string().c_str());
        testNode.model->transform.position = glm::vec3(0, 0, -40);
        testNode.model->transform.scale = glm::vec3(1.0, -1.0, 1.0);
        testNode.model->transform.rotation = glm::vec3(0, 0, 0);

        SceneNode cubeNode;
        cubeNode.model = std::make_unique<Model>(std::filesystem::path("../assets/models/sponza/sponza.obj").string().c_str());

        Texture tex;
        tex.id = textureFromFile("background_ddn.tga", std::filesystem::path("../assets/textures").string().c_str());
        cubeNode.model->texturesLoaded.push_back(tex);
        cubeNode.model->meshes.at(0).textures.push_back(tex);

        cubeNode.model->transform.position = glm::vec3(0, 0, -10);
        cubeNode.model->transform.scale = glm::vec3(0.05, -0.05, 0.05);
        cubeNode.model->transform.rotation = glm::vec3(0, 0, 0);

        rootNode.childNodes.emplace("testNode", std::move(testNode));
        rootNode.childNodes.emplace("cubeNode", std::move(cubeNode));
    }

    void setUpRenderBatches() {
        auto& testNodeRef = rootNode.childNodes["testNode"];
        auto& cubeNodeRef = rootNode.childNodes["cubeNode"];

        Shader* shader = new Shader("../src/shaders/vertex_shaders/simple_vertex.vs", "../src/shaders/fragment_shaders/simple_fragment.fs");

        Material* mat1 = new Material(shader);
        mat1->bindMat4("projection", projectionMat);

        renderBatches[mat1].push_back(testNodeRef.model.get());
        renderBatches[mat1].push_back(cubeNodeRef.model.get());
    }

    void setUpFBOs() {
        // Depth
        glGenFramebuffers(1, &depthMapFBO);
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCR_WIDTH, SCR_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Render
        // Generate depth Render Buffer Object that will hold the depth buffer values for the render frame buffer
        glGenRenderbuffers(1, &renderDepthRBO);
        glBindRenderbuffer(GL_RENDERBUFFER, renderDepthRBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);

        // Generate the Frame Buffer Object and the texture that will be rendered on the renderQuad
        glGenFramebuffers(1, &renderFBO);
        glGenTextures(1, &renderTexture);
        glBindTexture(GL_TEXTURE_2D, renderTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  

        // Bind the renderTexture and renderDepthRBO on the renderFBO
        glBindFramebuffer(GL_FRAMEBUFFER, renderFBO);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderDepthRBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTexture, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void generateDepthMap() {
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);

        glm::vec3 lightPos = -directionalLight * 10.0f;
        glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));

        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT); 
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT); 

        depthShader->use();
        depthShader->set("lightSpaceMatrix", lightSpaceMatrix);

        for (const auto& [material, renderBatch] : renderBatches) {
            for (const auto& model : renderBatch) {
                depthShader->set("model", model->modelMatrix());
                model->draw(*depthShader);
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void renderQuad(Shader& shader, unsigned int renderTexture) {
        static unsigned int quadVAO = 0, quadVBO;
        if (quadVAO == 0) {
            float quadVertices[] = {
                -1.0, -1.0, 0.0, 0.0, 0.0,
                1.0, -1.0, 0.0, 1.0, 0.0,
                1.0,  1.0, 0.0, 1.0, 1.0,
                -1.0,  1.0, 0.0, 0.0, 1.0
            };
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }

        shader.use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderTexture);
        shader.set("renderTexture", 0);
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
        glBindVertexArray(0);
    }

};

#endif