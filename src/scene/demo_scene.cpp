#ifndef DEMO_SCENE_H
#define DEMO_SCENE_H

#include "scene.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/string_cast.hpp"

#include "../camera/basic_camera_controller.cpp"

class DemoScene : public Scene {

    private:
    unsigned int SCR_WIDTH;
    unsigned int SCR_HEIGHT;
    BasicCameraController camera;
    Shader shader;
    glm::mat4 projectionMat;

    public:
    DemoScene(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT) : 
                SCR_WIDTH(SCR_WIDTH), SCR_HEIGHT(SCR_HEIGHT),
                camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -1), glm::vec3(0), 0, -90, 75),
                shader("../src/shaders/vertex_shaders/simple_vertex.vs", "../src/shaders/fragment_shaders/simple_fragment.fs") 
    {
        
        projectionMat = glm::perspective(camera.fovY, (float) SCR_WIDTH / (float) SCR_HEIGHT, 0.1f, 100.0f);

        SceneNode testNode;

        std::string path = std::filesystem::path("../assets/models/guitar_backpack/backpack.obj").string();
        testNode.model = std::make_unique<Model>(path.c_str());
        testNode.model->transform.position = glm::vec3(0, 0, -75);
        testNode.model->transform.scale = glm::vec3(1.0, -1.0, 1.0);
        testNode.model->transform.rotation = glm::vec3(0, 0, 0);

        std::cout << glm::to_string(testNode.model->modelMatrix()) << std::endl;

        int counter = 0;
        for (auto mesh : testNode.model->meshes) {
            std::cout << "Mesh " << counter++ << std::endl;
            mesh.printVertices(10);
        }

        rootNode.childNodes.emplace("testNode", std::move(testNode));

        std::cout << "Loaded: 'demo_scene'!" << std::endl;
    }

    void render(float interPolation) override  {
        Model* model = rootNode.childNodes["testNode"].model.get();

        shader.use();
        shader.setMat4("view", camera.viewMatrix());
        shader.setMat4("projection", projectionMat);
        shader.setMat4("model", model->modelMatrix());

        model->draw(shader);
    }

    void update(float dt, std::map<int, bool>& keyboard, glm::vec2 mouseMovement) override  {
        camera.update(dt, keyboard, mouseMovement);

        
    }
};

#endif