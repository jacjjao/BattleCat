#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <pch.hpp>
#include <string_view>

struct Rect {
    glm::vec2 pos;
    glm::vec2 size;
    glm::vec3 color;
};

class LoadingRectangles
{
private:
    struct Vertex {
        glm::vec2 pos;
        glm::vec4 color;
    };

public:
    LoadingRectangles();

    ~LoadingRectangles();

    void DrawRect(float x, float y, float w, float h, glm::vec4 color);

    void DrawAll();

private:
    GLuint m_VBO = 0;
    GLuint m_VAO = 0;
    GLuint m_Program = 0;
    std::vector<Vertex> m_Vertices;
    int m_Count = 0;
};

namespace RectProgram {

    // clang-format off
    static inline const char* vertex_shader = R"(
        #version 410 core

        layout(location = 0) in vec2 pos;
        layout(location = 1) in vec4 color;

        layout(location = 0) out vec4 o_color;
        
        uniform mat4 vp;

        void main() {
            gl_Position = vp * vec4(pos, 0, 1);
            o_color = color;
        }
    )";

    static inline const char* fragment_shader = R"(
        #version 410 core

        in vec4 color;
        
        out vec4 fragColor;

        void main() {
            fragColor = color;
        }
    )";

    // clang-format on

} // RectProgram

#endif