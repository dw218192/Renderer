#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <scene.h>
#include <shader.h>
#include <renderer.h>

int main() {
    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Test", nullptr, nullptr);
    if(!window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if(glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    Shader vs(ShaderType::Vertex);
    auto res = vs.from_src("\
        #version 330 core\n\
        layout (location = 0) in vec3 aPos;\n\
        void main() {\n\
            gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n\
        }\n\
    ");
    if(!res.valid()) {
        std::cerr << res.error() << std::endl;
        glfwTerminate();
        return -1;
    }
    Shader fs(ShaderType::Fragment);
    res = fs.from_src("\
        #version 330 core\n\
        layout(location = 0) out vec3 FragColor;\n\
        void main() {\n\
            FragColor = vec3(1.0f, 0.5f, 0.2f);\n\
        }\n\
    ");
    if(!res.valid()) {
        std::cerr << res.error() << std::endl;
        glfwTerminate();
        return -1;
    }

    ShaderProgram program;
    res = program.from_shaders(vs, fs);
    if(!res.valid()) {
        std::cerr << res.error() << std::endl;
        glfwTerminate();
        return -1;
    }

    Renderer renderer{ { 640, 480 } };
    res = renderer.open_scene(Scene::make_triangle_scene());
    if (!res.valid()) {
        std::cerr << res.error() << std::endl;
        glfwTerminate();
        return -1;
    }
    renderer.set_shader(program);

	auto renderRes = renderer.render();
    if(!renderRes.valid()) {
        std::cerr << renderRes.error() << std::endl;
        glfwTerminate();
        return -1;
    }

    auto&& renderImg = renderRes.value();
    renderImg.save("../out.png");

    //while (!glfwWindowShouldClose(window)) {
    //    glClear(GL_COLOR_BUFFER_BIT);
    //    {
    //        auto res = renderer.render();
    //        if (!res.valid()) {
    //            std::cerr << res.error() << std::endl;
    //            glfwTerminate();
    //            return -1;
    //        }
    //    }
    //    glfwSwapBuffers(window);
    //    glfwPollEvents();

    //    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    //        glfwSetWindowShouldClose(window, GLFW_TRUE);
    //    }
    //}

    glfwTerminate();
}