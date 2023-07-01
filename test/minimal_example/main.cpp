#include <application.h>
#include <iostream>


struct MinimalApp : public Application {
    MinimalApp(RenderConfig const& config, std::string_view const& name);
    void loop() override;
};

MinimalApp::MinimalApp(RenderConfig const& config, std::string_view const& name)
    : Application{ config, name }
{
    Scene scene;
    auto res = scene.from_obj_file("./armadillo.obj");
    if (!res.valid()) {
        std::cerr << res.error() << std::endl;
        Application::quit(-1);
    }
    auto open_res = get_renderer().open_scene(scene);
    if (!open_res.valid()) {
        std::cerr << open_res.error() << std::endl;
        Application::quit(-1);
    }
}

void MinimalApp::loop() {
	auto render_res = get_renderer().render();
    if(!render_res.valid()) {
        std::cerr << render_res.error() << std::endl;
        Application::quit(-1);
    }
    render_res.value().save("./out.png");
    Application::quit(0);
}

int main() {
    RenderConfig const config{
        1280, 720, // resolution
        60.0, // y field of view
        120.0 // frame rate cap
    };

    MinimalApp app{ config, "Minimal Example" };
    app.run();
    return 0;
}
