#include <application.h>
#include <iostream>


struct MinimalApp : public Application {
    MinimalApp(RenderConfig const& config, std::string_view const& name);
    void loop() override;
    int loop_cnt = 0;
};

MinimalApp::MinimalApp(RenderConfig const& config, std::string_view const& name)
    : Application{ config, name }
{
    Scene scene;
    check_error(scene.from_obj_file("./armadillo.obj"));
    check_error(get_renderer().open_scene(scene));
}

void MinimalApp::loop() {
    if(loop_cnt == 0) {
        auto&& render_res = check_error(get_renderer().render());
        render_res.save(FileFormat::PNG, "./out.png");
        render_res.save(FileFormat::JPG, "./out.jpg");

    	++loop_cnt;
    } else {
        Scene scene;
        check_error(scene.from_obj_file("./teapot.obj"));
        check_error(get_renderer().open_scene(scene));
        auto&& render_res =  check_error(get_renderer().render());
        render_res.save(FileFormat::BMP, "./out.bmp");
        render_res.save(FileFormat::TGA, "./out.tga");

        Application::quit(0);
    }
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
