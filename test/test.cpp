#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <scene.h>
#include <shader.h>
#include <renderer.h>

constexpr int width = 640;
constexpr int height = 480;
constexpr double motion_scale = 2.0;

bool first_time_motion = false;
int cur_button_down = -1;

static void clickFunc(GLFWwindow* window, int button, int action, int mods) {
    (void)window;
    (void)mods;

    cur_button_down = button;
    if (action == GLFW_PRESS) {
        first_time_motion = true;
    } else if (action == GLFW_RELEASE) {
        cur_button_down = -1;
    }
}

static void motionFunc(GLFWwindow* window, double mouse_x, double mouse_y) {
	static double prev_mouse_x, prev_mouse_y;
    if(cur_button_down == -1) {
        return;
    }

    auto const rend = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    if (!first_time_motion) {
        double px = motion_scale * (mouse_x - prev_mouse_x) / width;
        double py = motion_scale * (mouse_y - prev_mouse_y) / height;

    	Cmd cmd;
        if(cur_button_down == GLFW_MOUSE_BUTTON_LEFT) {
            cmd = Cmd_CameraMove{ {px, -py, 0 } };
        } else if (cur_button_down == GLFW_MOUSE_BUTTON_RIGHT) {
            cmd = Cmd_CameraRot{ {py, px, 0 } };
        } else if (cur_button_down == GLFW_MOUSE_BUTTON_MIDDLE) {
            cmd = Cmd_CameraZoom{ static_cast<real>(py) * 10 };
        }

        auto const res = rend->exec(cmd);
        if (!res.valid()) {
            std::cerr << res.error() << std::endl;
            return;
        }
    } else {
        first_time_motion = false;
    }
    prev_mouse_x = mouse_x;
    prev_mouse_y = mouse_y;
}

static void scrollFunc(GLFWwindow* window, double x_offset, double y_offset) {
    (void)x_offset;
    auto const rend = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
    auto const res = rend->exec(Cmd_CameraZoom{static_cast<real>(y_offset)});
    if(!res.valid()) {
        std::cerr << res.error() << std::endl;
    }
}

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
    RenderConfig config{
        width, height,
        60.0,
		Transform::look_at(vec3(0,0,2), vec3(0, 0, 0), vec3(0, 1, 0))
    };

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
    Renderer renderer{ config };
	{
        auto const res = renderer.open_scene(scene);
        if (!res.valid()) {
            std::cerr << res.error() << std::endl;
            glfwTerminate();
            return -1;
        }
    }

    if constexpr (false)
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

    glfwSetWindowUserPointer(window, &renderer);

    glfwSetMouseButtonCallback(window, clickFunc);
    glfwSetCursorPosCallback(window, motionFunc);
    glfwSetScrollCallback(window, scrollFunc);

    while (!glfwWindowShouldClose(window)) {
        {
            auto res = renderer.render();
            if (!res.valid()) {
                std::cerr << res.error() << std::endl;
                glfwTerminate();
                return -1;
            }
            if(auto res2 = res.value().upload_to_frame_buffer(); !res2.valid()) {
                std::cerr << res2.error() << std::endl;
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

}