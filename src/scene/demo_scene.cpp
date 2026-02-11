#ifndef DEMO_SCENE_H
#define DEMO_SCENE_H

#include "scene.h"

#include "../camera/basic_camera_controller.cpp"

class DemoScene : public Scene {

    private:
    BasicCameraController camera;
    Shader shader;
    glm::mat4 projection;

    public:
    DemoScene() : camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -1), glm::vec3(0), 0, 0, 75),
                  shader("../shaders/vertex_shaders/simple_vertex.vs", "../shaders/fragment_shaders/simple_fragment.fs") 
    {
        
        // TODO: Fix projection
        projection = glm::perspective((float) 75, (float) 1280 / (float) 720, 0.1f, 100.0f);

        SceneNode testNode;

        std::string path = std::filesystem::path("../assets/models/guitar_backpack/backpack.obj").string();
        testNode.model = std::make_unique<Model>(path.c_str());

        rootNode.childNodes.emplace("testNode", std::move(testNode));

        std::cout << "Loaded: 'demo_scene'!" << std::endl;
    }

    void render(float interPolation) override  {
        Model* model = rootNode.childNodes["backpack"].model.get();

        // shader.use();
        // shader.setMat4("view", camera.viewMatrix());
        // shader.setMat4("projection", projection);
        // shader.model

        // model->draw(shader);
    }

    void update(float dt, std::map<int, bool>& keyboard, glm::vec2 mouseMovement) override  {
        camera.update(dt, keyboard, mouseMovement);

        
    }
};

#endif