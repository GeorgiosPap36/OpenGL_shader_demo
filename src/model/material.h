#ifndef MATERIAL_H
#define MATERIAL_H

#include <map>
#include <variant>

#include "../shaders/shader.h"

class Material {
    public:
    Shader* shader;
    using UniformValue = std::variant<
        int,
        float,
        glm::vec3,
        glm::mat4
    >;
    std::map<std::string, UniformValue> uniforms;

    Material(Shader* shader);

    void bindUniforms();

    void bindInt(const std::string& name, int i);
    void bindFloat(const std::string& name, float f);
    void bindVec3(const std::string& name,  glm::vec3& vec3);
    void bindMat4(const std::string& name,  glm::mat4& mat4);
    
};

#endif