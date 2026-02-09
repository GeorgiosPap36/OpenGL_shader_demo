#ifndef DEMO_SCENE_H
#define DEMO_SCENE_H

#include <memory>
#include <iostream>

#include "scene.h"

#include "../camera/basic_camera_controller.cpp"

class DemoScene : public Scene {

    private:
    BasicCameraController camera;

    public:
    DemoScene() : camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -1), glm::vec3(0), 0, 0, 75) {

        std::cout << "Loaded: 'demo_scene'!" << std::endl;
    }

    void render(float interPolation) override  {
        
    }

    void update(float dt, std::map<int, bool>& keyboard, glm::vec2 mouseMovement) override  {
        camera.update(dt, keyboard, mouseMovement);

        
    }
};

#endif