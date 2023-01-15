#version 330 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texture_coords;

uniform sampler2D u_color_texture;

out vec4 f_color;

void main()
{
    vec3 light_direction = vec3(0.0, -1.0, -1.0);
    vec3 color = texture(u_color_texture, v_texture_coords).xyz;
    vec3 ambient = vec3(0.2, 0.2, 0.2) * color;
    vec3 diffuse = vec3(0.7, 0.7, 0.7) * max(dot(normalize(v_normal), -normalize(light_direction)), 0.0) * color;
    
    color = ambient + diffuse;
    f_color = vec4(color, 1.0);
}