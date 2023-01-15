#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in float curvature;

uniform mat4 u_world_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

out vec3 v_position;
out vec3 v_normal;
out float v_curvature;

void main()
{
    vec3 shifted_position = position - 0.0001 * normal; // to avoid z-fighting with wireframe
    v_normal = (u_world_matrix * vec4(normalize(normal), 0.0)).xyz;
    v_curvature = curvature * (1/100.0);
    gl_Position = u_projection_matrix * u_view_matrix * u_world_matrix * vec4(shifted_position, 1.0);
}