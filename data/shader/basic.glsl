#version 330 core

#ifdef VERTEX_SHADER

// vertex attributes
layout (location = 0) in vec3 position;

// per instance vertex attributes
layout(location = 1) in vec3 col_0;
layout(location = 2) in vec3 col_1;
layout(location = 3) in vec3 col_2;
layout(location = 4) in vec3 col_3;

uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

out vec3 v_position;

void main()
{
    mat4 world_matrix = mat4(vec4(col_0, 0.0), vec4(col_1, 0.0), vec4(col_2, 0.0), vec4(col_3, 1.0));
    v_position = (world_matrix * vec4(position, 1.0)).xyz;
    gl_Position = u_projection_matrix * u_view_matrix * world_matrix * vec4(position, 1.0);
}

#endif

#ifdef FRAGMENT_SHADER

in vec3 v_position;

out vec4 f_color;

void main()
{
    f_color = vec4(v_position, 1.0);
}

#endif