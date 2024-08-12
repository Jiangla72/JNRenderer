#include "SoftRenderer.h"
#include"Triangle.h"
#include"Scene.h"
#include"Camera.h"
#include"Model.h"
#include "Mesh.h"
#include<array>
#include <algorithm>
#include "Input.h"
#include <GLFW/glfw3.h>

SoftRenderer::SoftRenderer()
{
}

SoftRenderer::~SoftRenderer()
{
   
}

void SoftRenderer::init(std::shared_ptr<Scene> scene,int width, int height)
{
    m_pScene = scene;
    m_nWidth = width;
    m_nHeight = height;
    depth_buf.resize(width * height);
    frame_buf = new GLubyte[width * height*3];
    glViewport(0, 0, 800, 600);
    glGenTextures(1, &m_backBuffer);
    glBindTexture(GL_TEXTURE_2D, m_backBuffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_2D, 0);

    const char* vertexShaderSource = "#version 330 core\n"
       
   
        "layout (location = 0) in vec3 aPos;\n"
        "layout(location = 1) in vec2 aTexCoord;\n"
        "out vec2 TexCoord;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0);\n"
        "  TexCoord = aTexCoord;\n"
        "}\0";
    const char* fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec2 TexCoord;\n"
        "uniform sampler2D defaultSampler;\n"
        "void main()\n"
        "{\n"
        "   FragColor = texture(defaultSampler, TexCoord);\n"
        "   //FragColor = vec4(1.0f,1.0f,1.0f, 1.0);\n"
        "}\n\0";

    // build and compile our shader program
      // ------------------------------------
      // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        //     ---- 位置 ----     - 纹理坐标 -
             1.f,  1.f, 0.0f,  1.0f, 1.0f,   // 右上
             1.f, -1.f, 0.0f,  1.0f, 0.0f,   // 右下
            -1.f, -1.f, 0.0f,  0.0f, 0.0f,   // 左下
            -1.f,  1.f, 0.0f,  0.0f, 1.0f    // 左上
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    auto err = glGetError();
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

}

void SoftRenderer::render()
{
    clear();
    draw();
}

void SoftRenderer::present()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBindTexture(GL_TEXTURE_2D, m_backBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLint)m_nWidth, (GLint)m_nHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, (unsigned char*)frame_buf);
    glBindTexture(GL_TEXTURE_2D, 0);
    // draw our first triangle
    glUseProgram(shaderProgram);

    glBindTexture(GL_TEXTURE_2D, m_backBuffer);
 
   
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    auto err = glGetError();
   // glDrawPixels(m_nWidth, m_nHeight, GL_RGB, GL_UNSIGNED_BYTE, frame_buf);
    glFlush();
}

void SoftRenderer::update()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrameTime;
    lastFrameTime = currentFrame;

    glm::vec3 moveDir = { 0.0f,0.0f,0.0f };
	float cameraSpeed = 0.1f * deltaTime;
    Camera* l_Camera = m_pScene->getCamera();
    CameraData cameraData = l_Camera->getCameraData();
    glm::vec3 cameraPos = cameraData.pos;
    if (Input::getKey(GLFW_KEY_UP) || Input::getKey(GLFW_KEY_W))
    {
        moveDir += cameraData.forward * (1.0f);
    }
    if (Input::getKey(GLFW_KEY_LEFT) || Input::getKey(GLFW_KEY_A))
    {
        moveDir += glm::normalize(glm::cross(cameraData.forward, cameraData.up)) * (-1.0f);
    }
    if (Input::getKey(GLFW_KEY_DOWN) || Input::getKey(GLFW_KEY_S))
    {
        moveDir += cameraData.forward * (-1.0f);
    }
    if (Input::getKey(GLFW_KEY_RIGHT) || Input::getKey(GLFW_KEY_D))
    {
        moveDir += glm::normalize(glm::cross(cameraData.forward, cameraData.up)) * (1.0f);
    }
    if (Input::getKey(GLFW_KEY_Q))
    {
        moveDir += cameraData.up * (1.0f);
    }
    if (Input::getKey(GLFW_KEY_E))
    {
        moveDir += cameraData.up * (-1.0f);
    }
    cameraPos += cameraSpeed * moveDir;
    l_Camera->setPos(cameraPos);

    if (Input::mouseMoving && Input::getMouseButton(1))
    {
        float xoffset = Input::mouseMotion.x;
        float yoffset = -Input::mouseMotion.y;
        float sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;
        cameraData.pitch += yoffset;
        cameraData.yaw += xoffset;
        l_Camera->setPitchAndYaw(cameraData.pitch + yoffset, cameraData.yaw + xoffset);
        Input::mouseMoving = false;
    }
}

void SoftRenderer::clear()
{
    std::fill(depth_buf.begin(), depth_buf.end(), -std::numeric_limits<float>::infinity());
    std::fill(frame_buf, frame_buf+ m_nWidth * m_nHeight * 3 , 0);
}

void SoftRenderer::draw()
{
    float f1 = (50 - 0.1) / 2.0;
    float f2 = (50 + 0.1) / 2.0;
    Camera* l_Camera = m_pScene->getCamera();
    glm::mat4x4 matView = l_Camera->getCameraData().matView;
    glm::mat4x4 matModel(1.0f);
    glm::mat4x4 mvp = glm::mat4x4(1)*l_Camera->getCameraData().matViewProject;
    for (const auto& model : m_pScene->getModels())
    {
        auto& meshes = model->m_vecMesh;
        for (const auto& m : meshes)
        {

            auto& TriangleList = m->triangles;
            for (const auto& t : TriangleList)
            {
                Triangle newtri = *t;

                std::array<glm::vec4, 3> mm{
                        (matView * matModel * t->v[0]),
                        (matView * matModel * t->v[1]),
                        (matView * matModel * t->v[2])
                };

                std::array<glm::vec3, 3> viewspace_pos;

                std::transform(mm.begin(), mm.end(), viewspace_pos.begin(), [](glm::vec4& v) {
                    return glm::vec3(v);
                    });

                glm::vec4 v[] = {
                        mvp * t->v[0],
                        mvp * t->v[1],
                        mvp * t->v[2]
                };
                //Homogeneous division
                for (auto& vec : v) {
                    vec.x /= vec.w;
                    vec.y /= vec.w;
                    vec.z /= vec.w;
                }

                glm::mat4x4 inv_trans = glm::transpose(glm::inverse(matView * matModel));
                glm::vec4 n[] = {
                        inv_trans * glm::vec4(t->normal[0], 0.0f),
                        inv_trans * glm::vec4(t->normal[1], 0.0f),
                        inv_trans * glm::vec4(t->normal[2], 0.0f)
                };


                if(std::abs(v[0].z) >1.f && std::abs(v[1].z) > 1.f&& std::abs(v[2].z) > 1.f)
                {
                    continue;
                }

                //Viewport transformation
                for (auto& vert : v)
                {
                    vert.x = 0.5 * m_pScene->width * (vert.x + 1.0);
                    vert.y = 0.5 * m_pScene->height * (vert.y + 1.0);
                    vert.z = vert.z * f1 + f2;
                }

                //screen space coordinates
                for (int i = 0; i < 3; ++i)
                {
                    newtri.setVertex(i, v[i]);
                }

                //view space normal
               // newtri.normal = glm::vec3(n[0]);

                //newtri.setColor(0, 148, 121.0, 92.0);
                //newtri.setColor(1, 148, 121.0, 92.0);
                //newtri.setColor(2, 148, 121.0, 92.0);

                // Also pass view space vertice position

                
                rasterize(newtri, viewspace_pos);

            }
        }
    }
}

static bool insideTriangle(int x, int y, const glm::vec4* _v) {
    glm::vec3 v[3];
    for (int i = 0; i < 3; i++)
        v[i] = { _v[i].x,_v[i].y, 1.0 };
    glm::vec3 f0, f1, f2;
    f0 =glm::cross(v[1],v[0]);
    f1 = glm::cross(v[2],v[1]);
    f2 = glm::cross(v[0],v[2]);
    glm::vec3 p(x, y, 1.);
    if ((glm::dot(p,f0) * glm::dot(f0,v[2]) > 0) && (glm::dot(p,f1) * glm::dot(f1,v[0]) > 0) && (glm::dot(p,f2) * glm::dot(f2,v[1]) > 0))
        return true;
    return false;
}

static std::tuple<float, float, float> computeBarycentric2D(float x, float y, const glm::vec4* v) {
    float c1 = (x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * y + v[1].x * v[2].y - v[2].x * v[1].y) / (v[0].x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * v[0].y + v[1].x * v[2].y - v[2].x * v[1].y);
    float c2 = (x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * y + v[2].x * v[0].y - v[0].x * v[2].y) / (v[1].x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * v[1].y + v[2].x * v[0].y - v[0].x * v[2].y);
    float c3 = (x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * y + v[0].x * v[1].y - v[1].x * v[0].y) / (v[2].x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * v[2].y + v[0].x * v[1].y - v[1].x * v[0].y);
    return { c1,c2,c3 };
}

void SoftRenderer::rasterize(const Triangle& t, const std::array<glm::vec3, 3>& view_pos)
{
	const glm::vec4* v = t.v;
    float minX = std::floor(std::min(std::min(v[0].x, v[1].x), v[2].x));
    float maxX = std::ceil(std::max(std::max(v[0].x, v[1].x), v[2].x));
    float minY = std::floor(std::min(std::min(v[0].y, v[1].y), v[2].y));
    float maxY = std::ceil(std::max(std::max(v[0].y, v[1].y), v[2].y));
    minX = std::max(.0f, minX);
    maxX = std::min(maxX, (float)(m_nWidth-1));
    minY = std::max(.0f, minY);
    maxY = std::min(maxY, (float)(m_nHeight-1));
    for (size_t i = minX; i <= maxX; i++)
    {
        for (size_t j = minY; j <= maxY; j++) {
            if (insideTriangle(i, j, v))
            {
                auto table = computeBarycentric2D(i, j, v);
                float alpha = std::get<0>(table), beta = std::get<1>(table), gamma = std::get<2>(table);
                float w_reciprocal = 1.0 / (alpha / v[0].w + beta / v[1].w + gamma / v[2].w);
                float z_interpolated = alpha * v[0].z / v[0].w + beta * v[1].z / v[1].w + gamma * v[2].z / v[2].w;
                z_interpolated *= w_reciprocal;

                glm::vec3 interpolated_normal = glm::normalize(alpha * t.normal[0] + beta * t.normal[1] + gamma * t.normal[2]);
                glm::vec2 interpolated_texcoords = alpha * t.tex_coords[0] + beta * t.tex_coords[1] + gamma * t.tex_coords[2];
                if (interpolated_texcoords.x < 0)
                {
                    interpolated_texcoords.x = 0;
                }
                if (interpolated_texcoords.y < 0)
                {
                    interpolated_texcoords.y = 0;
                }
                glm::vec3 interpolated_shadingcoords = alpha * view_pos[0] + beta * view_pos[1] + gamma * view_pos[2];
                auto ind = (j) * m_nWidth + i;
                if (z_interpolated > depth_buf[ind])
                {
                    depth_buf[ind] = z_interpolated;
                    glm::vec3 color = texture_fragment_shader(interpolated_shadingcoords, interpolated_normal);
                    frame_buf[ind * 3] = color.x;
                    frame_buf[ind * 3 + 1] = color.y;
                    frame_buf[ind * 3 + 2] = color.z;

                }
            }
        }
    }
}

glm::vec3 SoftRenderer::texture_fragment_shader(glm::vec3 iPos, glm::vec3 iNormal)
{
    glm::vec3 return_color = { 0, 0, 0 };
    //if (payload.texture)
    //{
    //    // TODO: Get the texture value at the texture coordinates of the current fragment
    //    return_color = payload.texture->getColor(payload.tex_coords.x(), payload.tex_coords.y());
    //}
    //glm::vec3 texture_color;
    //texture_color << return_color.x, return_color.y, return_color.z;
    glm::vec3 color = { 0.5f,0.5f,0.5f };
    glm::vec3 point = iPos;
    glm::vec3 normal = iNormal;

    glm::vec3 ka = glm::vec3(0.005, 0.005, 0.005);
    glm::vec3 kd = color;
    glm::vec3 ks = glm::vec3(0.7937, 0.7937, 0.7937);

    auto l1 = light{ {20, 20, 20}, {500, 500, 500} };
    auto l2 = light{ {-20, 20, 0}, {500, 500, 500} };

    std::vector<light> lights = { l1, l2 };
    glm::vec3 amb_light_intensity{ 10, 10, 10 };
    glm::vec3 eye_pos{ 0, 0,0 };

    float p = 150;

    glm::vec3 result_color = { 0, 0, 0 };

    for (auto& light : lights)
    {
        // TODO: For each light source in the code, calculate what the *ambient*, *diffuse*, and *specular* 
        // components are. Then, accumulate that result on the *result_color* object.
        glm::vec3 ld = light.position - point;
        float r2 = glm::dot(ld,ld);
        ld = glm::normalize(ld);
        glm::vec3 diffuse = kd*(light.intensity * (1 / r2)) * std::max(0.0f, glm::dot(normal,ld));
        glm::vec3 h = glm::normalize(glm::normalize(eye_pos - point) + ld);
        glm::vec3 specular = ks*(light.intensity * (1 / r2)) * std::max(0.0f, std::pow(glm::dot(normal,h), p));
        glm::vec3 ambient = ka*(amb_light_intensity);
        result_color += diffuse + specular + ambient;
    }

    return result_color * 255.f;
}
