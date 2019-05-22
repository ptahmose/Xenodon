#ifndef _XENODON_RAYTRACE_SHADER_GLSL
#define _XENODON_RAYTRACE_SHADER_GLSL

// Define common structures, bindings and functions that every shader needs

struct Camera {
    vec4 forward;
    vec4 up;
    vec4 translation;
};

struct Rect {
    ivec2 offset;
    uvec2 extent;
};

struct RenderParameters {
    vec4 voxel_ratio;
    uvec4 model_dim;
    float density;
};

layout(local_size_x = 8, local_size_y = 8) in;

layout(push_constant) uniform PushConstant {
    Camera camera;
    float time;
} push;

layout(binding = 0) readonly uniform UniformBuffer {
    Rect output_region;
    Rect display_region;
    RenderParameters params;
} uniforms;

layout(binding = 1, rgba8) restrict writeonly uniform image2D render_target;

vec3 ray(vec3 dir, vec3 up, vec2 uv) {
    uv -= 0.5;
    uv.y *= float(uniforms.display_region.extent.y) / float(uniforms.display_region.extent.x);

    vec3 right = normalize(cross(up, dir));
    up = normalize(cross(right, dir));

    return normalize(uv.x * right + uv.y * up + dir);
}

#endif