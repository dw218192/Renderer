# Overview
This is a simple renderer library that supports the following functionalities:
1. render images from a single .obj mesh file.
2. accept user commands to change the view.

# Requirement
1. Operating Systems: Windows or Linux
3. (If on Windows) Visual Studio 2019 or above
4. (If on Linux) CMake 3.11 or above, Make, GCC/G++ 7.3.0 or above

# How to Build
1. Clone the repository
2. Make a build directory in the top level directory: `mkdir build`
3. Change to the build directory: `cd build`
4. Run cmake to configure the project: `cmake ..`
5. Build the project
    * On Windows, open the generated .sln file in Visual Studio and build the project.
    * On Linux, run `make` in the build directory.

# Example
- For a complete GUI application example, please refer to the `test/gui_example` directory.
- The following code snippet shows a quick way to render an image from a .obj file.
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

- In the `loop()` method, you may do some rendering, such as rendering the scene and swap the buffers.
- Here we'll just render the scene once, save it to a file, and quit the application.

```cpp
void MinimalApp::loop() {
	auto render_res = get_renderer().render();
    if(!render_res.valid()) {
        std::cerr << render_res.error() << std::endl;
        Application::quit(-1);
    }
    render_res.value().save("./out.png");
    Application::quit(0);
}
```