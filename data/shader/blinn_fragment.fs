#version 330 core

#define PI 3.1415926
#define K 100
#define LIGHT_DIR vec3(0.0, 0.0, 1.0)

in vec3 v_position;
in vec3 v_normal;
in float v_curvature;

uniform vec3 u_color;
uniform vec3 u_camera_position;

out vec4 f_color;

void main()
{
    //vec3 color = vec3(v_curvature, v_curvature, v_curvature);
    vec3 color = u_color;

    vec3 to_camera = normalize(u_camera_position - v_position);
    vec3 halfway = normalize(-normalize(LIGHT_DIR) + to_camera);
    float theta = max(dot(normalize(v_normal), halfway), 0.0);
    
    vec3 ambient    = color * vec3(0.4, 0.4, 0.4);
    vec3 diffuse    = color * (1/PI) * dot(normalize(v_normal), -normalize(LIGHT_DIR));
    vec3 specular   = color * (((K + 8)/(8 * PI)) * pow(theta, K));

    vec3 final_color = ambient + diffuse + specular;

    f_color = vec4(final_color, 1.0);
}