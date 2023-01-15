#version 330 core

// vertex attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texture_coords;

// per instance vertex attributes
layout(location = 3) in vec3 col_0;
layout(location = 4) in vec3 col_1;
layout(location = 5) in vec3 col_2;
layout(location = 6) in vec3 col_3;

uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_texture_coords;

void main()
{
    mat4 world_matrix = mat4(vec4(col_0, 0.0), vec4(col_1, 0.0), vec4(col_2, 0.0), vec4(col_3, 1.0));
    v_position = (world_matrix * vec4(position, 1.0)).xyz;
    v_normal = normal;
    v_texture_coords = texture_coords;
    gl_Position = u_projection_matrix * u_view_matrix * world_matrix * vec4(position, 1.0);
}