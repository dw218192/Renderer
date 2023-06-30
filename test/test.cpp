#include <iostream>

#include <ext.h>
#include <scene.h>
#include <renderer.h>
#include <application.h>
#include <imgui/guiFileDialog.h>

constexpr float k_init_move_sensitivity = 2.0;
constexpr float k_init_rot_sensitivity = 40.0;
constexpr float k_init_zoom_sensitivity = 10.0;

struct TestApp : public Application {
	TestApp(RenderConfig const& config, std::string_view const& name)
		: Application{config, name} {}

	void cursor_moved(double x, double y) override;
	void mouse_clicked(int button, int action, int mods) override;
	void mouse_scroll(double x, double y) override;
	void loop() override;

private:
    bool first_time_motion = true;
    int cur_button_down = -1;
    double prev_x = 0, prev_y = 0;
    std::string cur_obj_file;

    void draw_imgui();
    struct {
        float move_sensitivity = k_init_move_sensitivity;
        float rot_sensitivity = k_init_rot_sensitivity;
        float zoom_sensitivity = k_init_zoom_sensitivity;
        ImGui::FileDialogue dialogue{ "Dialogue", false, ".obj" };
    } m_control_state;
};

void TestApp::cursor_moved(double x, double y) {
    if (cur_button_down == -1) {
        return;
    }

    if (!first_time_motion) {
        auto px = (x - prev_x) / get_window_width();
        auto py = (y - prev_y) / get_window_height();

        Cmd cmd;
        auto move_sensitivity = m_control_state.move_sensitivity;
        auto rot_sensitivity = m_control_state.rot_sensitivity;
        auto zoom_sensitivity = m_control_state.zoom_sensitivity;

        if (cur_button_down == GLFW_MOUSE_BUTTON_LEFT) {
            cmd = Cmd_CameraMove{
	            {
                    move_sensitivity * px, -move_sensitivity * py, 0
	            }
            };
        }
        else if (cur_button_down == GLFW_MOUSE_BUTTON_RIGHT) {
            cmd = Cmd_CameraRot{
            	{
                    rot_sensitivity * py, rot_sensitivity * px, 0
            	}
            };
        }
        else if (cur_button_down == GLFW_MOUSE_BUTTON_MIDDLE) {
            cmd = Cmd_CameraZoom{
            	static_cast<real>(py * zoom_sensitivity)
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


void TestApp::draw_imgui() {
    ImGui::Begin("Renderer");

    ImGui::Spacing();
    auto path = ImGui::OpenFileDialogue(m_control_state.dialogue, "Open an .obj File");
    if (!path.empty() && path != cur_obj_file) {
        Scene scene;
        {
            auto const res = scene.from_obj_file(path);
            if (!res.valid()) {
                std::cerr << res.error() << std::endl;
                Application::quit(-1);
            }
        }

        auto const res = get_renderer().open_scene(scene);
        if (!res.valid()) {
            std::cerr << res.error() << std::endl;
            Application::quit(-1);
        }

        cur_obj_file = path;
    }

    if(ImGui::CollapsingHeader("Control Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Camera Control");
        ImGui::SliderFloat("Move Sensitivity", &m_control_state.move_sensitivity, 1.0f, 10.0f);
        ImGui::SliderFloat("Rotate Sensitivity", &m_control_state.rot_sensitivity, 2.0f, 100.0f);
        ImGui::SliderFloat("Zoom Sensitivity", &m_control_state.zoom_sensitivity, 1.0f, 20.0f);
    }

    ImGui::End();
}

void TestApp::loop() {
    draw_imgui();

    if(!get_renderer().valid()) {
        return;
    }

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
        60.0, 120.0
    };
    TestApp app{ config, "Test App" };
    app.run();
}