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

    Scene scene;
    {
        auto const res = Scene::make_triangle_scene();
        if(!res.valid()) {
            std::cerr << res.error() << std::endl;
            glfwTerminate();
            return -1;
        }
        scene = res.value();
    }
    Renderer renderer{ { 640, 480 } };
	{
        auto const res = renderer.open_scene(scene);
        if (!res.valid()) {
            std::cerr << res.error() << std::endl;
            glfwTerminate();
            return -1;
        }
    }
    {
        auto const res = renderer.render();
        if (!res.valid()) {
            std::cerr << res.error() << std::endl;
            glfwTerminate();
            return -1;
        }

        auto&& renderImg = res.value();
        renderImg.save("../out.png");
    }
    glfwTerminate();
    return 0;

#ifdef x
    while (!glfwWindowShouldClose(window)) {
        {
            auto res = renderer.render();
            if (!res.valid()) {
                std::cerr << res.error() << std::endl;
                glfwTerminate();
                return -1;
            }
        }
        glfwSwapBuffers(window);
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }

    glfwTerminate();
#endif
}