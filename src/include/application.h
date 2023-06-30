#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "scene.h"
#include "shader.h"
#include "renderer.h"
#include "ext.h"

// simple wrapper to make it easier to create a GUI application
struct Application {
    Application(RenderConfig const& config, std::string_view name);
    Application(Application const&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application const&) = delete;
    Application& operator=(Application&&) = delete;
    virtual ~Application();
    void run();
    [[nodiscard]] auto get_renderer() -> Renderer& { return m_renderer; }
    [[nodiscard]] auto get_window_width() const noexcept -> int;
    [[nodiscard]] auto get_window_height() const noexcept -> int;

	virtual void cursor_moved([[maybe_unused]] double x, [[maybe_unused]] double y) {  }
    virtual void mouse_clicked([[maybe_unused]] int button, [[maybe_unused]] int action, [[maybe_unused]] int mods) { }
    virtual void mouse_scroll([[maybe_unused]] double x, [[maybe_unused]] double y) { }
    virtual void loop() {}

	static void quit(int code);

private:
    GLFWwindow* m_window;
    Renderer m_renderer;
};