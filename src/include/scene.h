#pragma once
#include "object.h"
#include "result.h"

#include <string_view>
#include <vector>

struct Scene {
    [[nodiscard]] auto from_obj_file(std::string_view filename) noexcept -> Result<void>;
    [[nodiscard]] auto objects() const noexcept -> std::vector<Object> const& { return m_objects; }

	// compute good positions to place light and camera
	[[nodiscard]] auto get_good_cam_start() const noexcept -> Transform;
    [[nodiscard]] auto get_good_light_pos() const noexcept -> vec3;

    [[nodiscard]] auto begin_draw() const noexcept -> Result<void>;

	// for test only
    static auto make_triangle_scene() noexcept -> Result<Scene>;
private:
    // for now all objects use this shader
    static inline ShaderProgram s_default_shader;


    static auto get_default_shader() noexcept -> Result<ShaderProgram const&>;
    AABB compute_scene_bound() const noexcept;


    // these are initialized when the scene is loaded
	std::vector<Object> m_objects;
};