#ifndef DEMO_SCENE_H
#define DEMO_SCENE_H

#include "scene.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "../model/material.h"

#include "../camera/basic_camera_controller.cpp"

class DemoScene : public Scene {

    private:
    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;
    BasicCameraController camera;
    Shader shader;
    glm::mat4 projectionMat;

    std::map<Material*, std::vector<Model*>> renderBatches;

    public:
    DemoScene(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT) : 
                SCR_WIDTH(SCR_WIDTH), SCR_HEIGHT(SCR_HEIGHT),
                camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -1), glm::vec3(0), 0, -90, 75),
                shader("../src/shaders/vertex_shaders/simple_vertex.vs", "../src/shaders/fragment_shaders/simple_fragment.fs") 
    {
        
        setUpScene();

        setUpRenderBatches();

        std::cout << "Loaded: 'demo_scene'!" << std::endl;
    }

    void render(float interPolation) override  {
        for (const auto& [material, renderBatch] : renderBatches) {
            Shader* matShader = material->shader;
            matShader->use();

            matShader->set("view", camera.viewMatrix());
            matShader->set("projection", projectionMat);

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
    }

    void update(float dt, std::map<int, bool>& keyboard, glm::vec2 mouseMovement) override {
        camera.update(dt, keyboard, mouseMovement);

        
    }

    private:
    void setUpScene() {
        projectionMat = glm::perspective(camera.fovY, (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);

        SceneNode testNode;

        testNode.model = std::make_unique<Model>(std::filesystem::path("../assets/models/guitar_backpack/backpack.obj").string().c_str());
        testNode.model->transform.position = glm::vec3(0, 0, -75);
        testNode.model->transform.scale = glm::vec3(1.0, -1.0, 1.0);
        testNode.model->transform.rotation = glm::vec3(0, 0, 0);

        SceneNode cubeNode;
        cubeNode.model = std::make_unique<Model>(std::filesystem::path("../assets/models/cube/cube.obj").string().c_str());

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

        Material* mat1 = new Material(&shader);
        mat1->bindFloat("asd", 0.5);

        renderBatches[mat1].push_back(testNodeRef.model.get());
        renderBatches[mat1].push_back(cubeNodeRef.model.get());
    }
};

#endif