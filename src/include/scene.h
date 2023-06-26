#pragma once
#include "object.h"
#include "result.h"

#include <string_view>
#include <vector>

struct Scene {
    [[nodiscard]] auto from_obj_file(std::string_view filename) noexcept -> Result<void>;
    [[nodiscard]] auto objects() const noexcept -> std::vector<Object> const& { return m_objects; }
// for test only
    static auto make_triangle_scene() noexcept -> Result<Scene>;

private:
    static inline ShaderProgram s_default_shader;
    static auto get_default_shader() noexcept -> Result<ShaderProgram const&>;

    // these are initialized when the scene is loaded
	std::vector<Object> m_objects;
};