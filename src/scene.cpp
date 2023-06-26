#include "scene.h"

auto Scene::from_obj_file(std::string_view filename) noexcept -> Result<void> {
    auto const res = get_default_shader();
    if(!res.valid()) {
        return res.error();
    }

    Object obj{ res.value(), Transform{} };
    if(auto ores = obj.from_obj(filename); !ores.valid()) {
        return ores;
    }
    m_objects.emplace_back(std::move(obj));
    return Result<void>::ok();
}

auto Scene::make_triangle_scene() noexcept -> Result<Scene> {
    auto const res = get_default_shader();
    if (!res.valid()) {
        return res.error();
    }
    Scene scene;
    scene.m_objects.emplace_back(Object::make_triangle_obj(res.value(), Transform{ vec3{0,0,0}, vec3{0,0,0}, vec3{1,1,1} }));
    scene.m_objects.emplace_back(Object::make_triangle_obj(res.value(), Transform{ vec3{0.2,0.2,-1}, vec3{0,0,0}, vec3{1,1,1} }));
    return scene;
}

auto Scene::get_default_shader() noexcept -> Result<ShaderProgram const&> {
    if (!s_default_shader.valid()) {
        // initialize default shader program
        Shader vs(ShaderType::Vertex);
        {
            auto const res = vs.from_src(vs_obj_src);
            if (!res.valid()) {
                return res.error();
            }
        }
        Shader fs(ShaderType::Fragment);
        {
            auto const res = fs.from_src(ps_unicolor_src);
            if (!res.valid()) {
                return res.error();
            }
        }
        {
            auto const res = s_default_shader.from_shaders(std::move(vs), std::move(fs));
            if (!res.valid()) {
                return res.error();
            }
        }
    }
    return s_default_shader;
}
