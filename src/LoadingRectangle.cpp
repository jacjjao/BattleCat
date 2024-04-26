#include "LoadingRectangle.hpp"
#include "Util/Logger.hpp"
#include "Util/TransformUtils.hpp"
#include <glm/gtc/type_ptr.hpp>

LoadingRectangles::LoadingRectangles() {
    m_Vertices.reserve(10 * 6); // 10 deploy button, 6 vertex each rectangle

    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &RectProgram::vertex_shader, nullptr);
    glCompileShader(vertex_shader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, nullptr, infoLog);
        LOG_ERROR("Fail to compile CUSTOM vertex shader");
    }
    
    unsigned int frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &RectProgram::fragment_shader, nullptr);
    glCompileShader(frag_shader);
    
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(frag_shader, 512, nullptr, infoLog);
        LOG_ERROR("Fail to compile CUSTOM fragment shader");
    }
    
    m_Program = glCreateProgram();
    glAttachShader(m_Program, vertex_shader);
    glAttachShader(m_Program, frag_shader);
    glLinkProgram(m_Program);
    
    glGetProgramiv(m_Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(m_Program, 512, nullptr, infoLog);
        LOG_ERROR("Failed to link CUSTOM shader program");
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(frag_shader);
    
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.capacity() * sizeof(Vertex), nullptr,
                 GL_STREAM_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vertex::pos));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    auto t = Util::ConvertToUniformBufferData({}, {}, 0.0);
    glUseProgram(m_Program);
    glUniformMatrix4fv(glGetUniformLocation(m_Program, "vp"), 1, GL_FALSE,
                       glm::value_ptr(t.m_Projection));
    glUseProgram(0);
}

LoadingRectangles::~LoadingRectangles() {
    glDeleteBuffers(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteProgram(m_Program);
}

void LoadingRectangles::DrawRect(float x, float y, float w, float h,
                                 glm::vec4 color) {     
    const float half_w = w / 2.0f;
    const float half_h = h / 2.0f;
    Vertex tl;
    tl.pos.x = x - half_w;
    tl.pos.y = y + half_h;
    tl.color = color;
    m_Vertices.push_back(tl);
    Vertex tr;
    tr.pos.x = x + half_w;
    tr.pos.y = y + half_h;
    tr.color = color;
    m_Vertices.push_back(tr);
    Vertex bl;
    bl.pos.x = x - half_w;
    bl.pos.y = y - half_h;
    bl.color = color;
    m_Vertices.push_back(bl);
    
    m_Vertices.push_back(tr);
    m_Vertices.push_back(bl);
    Vertex br;
    br.pos.x = x + half_w;
    br.pos.y = y - half_h;
    br.color = color;
    m_Vertices.push_back(br);

    ++m_Count;
}

void LoadingRectangles::DrawAll() {
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_Vertices.size() * sizeof(Vertex),
                    m_Vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glUseProgram(m_Program);
    glBindVertexArray(m_VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_Vertices.size());
    glBindVertexArray(0);
    glUseProgram(0);

    m_Vertices.clear();
    m_Count = 0;
}

