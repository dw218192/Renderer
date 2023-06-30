#include <iostream>

#include <ext.h>
#include <scene.h>
#include <shader.h>
#include <renderer.h>
#include <application.h>

constexpr double k_init_move_sensitivity = 2.0;
constexpr double k_init_rot_sensitivity = 20.0;
constexpr double k_init_zoom_sensitivity = 10.0;

struct TestApp : public Application {
	TestApp(RenderConfig const& config, std::string_view const& name)
		: Application{config, name} { }

	void cursor_moved(double x, double y) override;
	void mouse_clicked(int button, int action, int mods) override;
	void mouse_scroll(double x, double y) override;
	void loop() override;

private:
    bool first_time_motion = true;
    int cur_button_down = -1;
    double prev_x = 0, prev_y = 0;
};

void TestApp::cursor_moved(double x, double y) {
    if (cur_button_down == -1) {
        return;
    }

    if (!first_time_motion) {
        double px = (x - prev_x) / get_window_width();
        double py = (y - prev_y) / get_window_height();

        Cmd cmd;
        if (cur_button_down == GLFW_MOUSE_BUTTON_LEFT) {
            cmd = Cmd_CameraMove{
	            {
                    k_init_move_sensitivity * px, -k_init_move_sensitivity * py, 0
	            }
            };
        }
        else if (cur_button_down == GLFW_MOUSE_BUTTON_RIGHT) {
            cmd = Cmd_CameraRot{
            	{
                    k_init_rot_sensitivity * py, k_init_rot_sensitivity * px, 0
            	}
            };
        }
        else if (cur_button_down == GLFW_MOUSE_BUTTON_MIDDLE) {
            cmd = Cmd_CameraZoom{
            	static_cast<real>(py * k_init_zoom_sensitivity)
            };
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
        1280, 720,
        60.0
    };
    TestApp app{ config, "Test App" };

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