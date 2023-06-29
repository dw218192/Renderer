#include <iostream>

#include <ext.h>
#include <scene.h>
#include <shader.h>
#include <renderer.h>
#include <application.h>

constexpr int width = 1280;
constexpr int height = 720;
constexpr double motion_scale = 2.0;


struct TestApp : public Application {
	TestApp(RenderConfig const& config, std::string_view const& name)
		: Application{config, name} { }

	void cursor_moved(double x, double y) override;
	void mouse_clicked(int button, int action, int mods) override;
	void mouse_scroll(double x, double y) override;
	void loop() override;

private:
    bool first_time_motion = false;
    int cur_button_down = -1;
    double prev_x = 0, prev_y = 0;
};

void TestApp::cursor_moved(double x, double y) {
    if (cur_button_down == -1) {
        return;
    }

    if (!first_time_motion) {
        double px = motion_scale * (x - prev_x) / width;
        double py = motion_scale * (y - prev_y) / height;

        Cmd cmd;
        if (cur_button_down == GLFW_MOUSE_BUTTON_LEFT) {
            cmd = Cmd_CameraMove{ {px, -py, 0 } };
        }
        else if (cur_button_down == GLFW_MOUSE_BUTTON_RIGHT) {
            cmd = Cmd_CameraRot{ {py, px, 0 } };
        }
        else if (cur_button_down == GLFW_MOUSE_BUTTON_MIDDLE) {
            cmd = Cmd_CameraZoom{ static_cast<real>(py) * 10 };
        }

        auto const res = get_renderer().exec(cmd);
        if (!res.valid()) {
            std::cerr << res.error() << std::endl;
            return;
        }
    }
    else {
        first_time_motion = false;
    }
    prev_x = x;
    prev_y = y;
}

void TestApp::mouse_clicked(int button, int action, int mods) {
    (void)mods;

    cur_button_down = button;
    if (action == GLFW_PRESS) {
        first_time_motion = true;
    }
    else if (action == GLFW_RELEASE) {
        cur_button_down = -1;
    }
}

void TestApp::mouse_scroll(double x, double y) {
    (void)x;
    real const delta = y < 0 ? REAL_LITERAL(-1.0) : REAL_LITERAL(1.0);
    auto const res = get_renderer().exec(Cmd_CameraZoom{ delta });
    if (!res.valid()) {
        std::cerr << res.error() << std::endl;
    }
}

void TestApp::loop() {
    auto res = get_renderer().render();
    if (!res.valid()) {
        std::cerr << res.error() << std::endl;
        quit(-1);
    }
    if (auto res2 = res.value().upload_to_frame_buffer(); !res2.valid()) {
        std::cerr << res2.error() << std::endl;
        quit(-1);
    }
}

int main() {
    RenderConfig const config{
        width, height,
        60.0,
		Transform::look_at(vec3(0,0,2), vec3(0, 0, 0), vec3(0, 1, 0))
    };
    TestApp app{ config, "Cao ni ma" };

    Scene scene;
    {
        auto const res = scene.from_obj_file("../_files/teapot.obj");
        // auto const res = Scene::make_triangle_scene();
        if(!res.valid()) {
            std::cerr << res.error() << std::endl;
            return -1;
        }
    }

    auto const res = app.get_renderer().open_scene(scene);
    if (!res.valid()) {
        std::cerr << res.error() << std::endl;
        return -1;
    }
    app.run();
}