# Overview
This is a simple renderer library that supports the following functionalities:
1. render images from a single .obj mesh file.
2. accept user commands to change the view.

# Requirement
1. Operating Systems: Windows or Linux
2. CMake 3.11 or above
3. Compiler toolchain with C++17 support
    - On Windows, Visual Studio 2017 or above is recommended.
    - On Linux, gcc 7.3 or above is recommended.

# How to Build
1. Clone the repository (Note: Please pass the `--recursive` flag to also clone the submodules)
    - In case you forgot to add `--recursive`, you can run `git submodule update --init --recursive` to clone the submodules after cloning the repository
2. Make a build directory in the top level directory: `mkdir build`
3. Change to the build directory: `cd build`
4. Run cmake to configure and generate the project: `cmake ..`
5. Build the project
    * On Windows, open the generated .sln file in Visual Studio and build the project.
    * On Linux, run `make` in the build directory.

# Example
- For a complete GUI application example, please refer to the `test/gui_example` directory.

## Error Handling
- All functions that may fail return a `Result<T,E>` object, where `T` is the return type and `E` is the error type (which is usually `std::string`).
- To check if a function call is successful, you may use the `valid()` method of the `Result` object.
- To get the return value of a successful function call, you may use the `value()` method of the `Result` object.

## Creating an application and Rendering
- This section will show you how to create a windowed application and render an image from a .obj file.
- First include the necessary header files under the `include` directory, and create a class that inherits from the `Application` class.

```cpp
#include "application.h"

struct MinimalApp : public Application {
    // ...
};
```

- At minimum, your class needs to implement the constructor and the `loop()` method.
    - see the `Application` class for more details.
- In the constructor, you may do some initialization, such as loading the .obj file and configure the renderer.

```cpp
MinimalApp::MinimalApp(RenderConfig const& config, std::string_view const& name)
    : Application{ config, name }
{
    Scene scene;
    auto res = scene.from_obj_file("path_to_your_obj_file");
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
```

- The `loop()` method is called every frame, and you may use it to update the scene and render it, or render ImGui widgets.
- Here we'll just render the scene once, save it to a file, and quit the application.

```cpp
void MinimalApp::loop() {
	auto res = get_renderer().render();
    if(!res.valid()) {
        std::cerr << res.error() << std::endl;
        Application::quit(-1);
    }
    
    auto render_res = res.value();
    render_res.save("./out.png");
    Application::quit(0);
}
```
- you may also retrieve the raw pixel data from the render result and do something else with it.
- the pixel is stored in RGB order, and each channel is stored in a byte (0-255).
```cpp
int w = render_res.get_width();
int h = render_res.get_height();
auto pixels = render_res.get_pixels();
for (int idx = 0; idx < w * h; ++idx) {
    byte r = pixels[3 * idx + 0];
    byte g = pixels[3 * idx + 1];
    byte b = pixels[3 * idx + 2];
    // do something with r, g, b
}
```

- To start the application, simply create an instance of your class and call the `run()` method.

```cpp
int main() {
    RenderConfig config{
        1280, 720, // resolution
        60.0, // y field of view
        120.0 // frame rate cap
    };

    MinimalApp app{ config, "MinimalApp" };
    app.run();
    return 0;
}
```

## Passing user commands to the renderer
- All user commands are passed to the renderer through the `Renderer::exec()` method.
- The `exec()` method takes any instance of the classes defined in `commands.h` as its argument.