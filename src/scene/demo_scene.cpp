#ifndef DEMO_SCENE_H
#define DEMO_SCENE_H

#include "scene.h"

#include "glm/gtx/string_cast.hpp"

#include "../camera/basic_camera_controller.cpp"

class DemoScene : public Scene {

    private:
    BasicCameraController camera;
    Shader shader;
    glm::mat4 projection;

    public:
    DemoScene() : camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -1), glm::vec3(0), 0, -90, 75),
                  shader("../src/shaders/vertex_shaders/simple_vertex.vs", "../src/shaders/fragment_shaders/simple_fragment.fs") 
    {
        
        // TODO: Fix projection
        projection = glm::perspective((float) 75, (float) 1280 / (float) 720, 0.1f, 100.0f);

        SceneNode testNode;

        std::string path = std::filesystem::path("../assets/models/guitar_backpack/backpack.obj").string();
        testNode.model = std::make_unique<Model>(path.c_str());
        testNode.model->transform.position = glm::vec3(0, 0, -75);
        testNode.model->transform.scale = glm::vec3(1.0f, -1.0f, 1.0f);
        testNode.model->transform.rotation = glm::vec3(0, 0, 0);

        std::cout << glm::to_string(testNode.model->modelMatrix()) << std::endl;

        rootNode.childNodes.emplace("testNode", std::move(testNode));

        std::cout << "Loaded: 'demo_scene'!" << std::endl;
    }

    void render(float interPolation) override  {
        Model* model = rootNode.childNodes["testNode"].model.get();

        shader.use();
        shader.setMat4("view", camera.viewMatrix());
        shader.setMat4("projection", projection);
        shader.setMat4("model", model->modelMatrix());

        model->draw(shader);
    }

    void update(float dt, std::map<int, bool>& keyboard, glm::vec2 mouseMovement) override  {
        camera.update(dt, keyboard, mouseMovement);

        
    }
};

#endif