#include "renderer.h"

Renderer::Renderer(Scene scene) : m_scene(std::move(scene)) {
    
}

void Renderer::exec(Cmd const& cmd) {

}

auto Renderer::render() -> Result<RenderResult> {
    return std::string("Not implemented");
}