#version 150

uniform mat4 mvp_matrix;
uniform float isPlane;

in vec4 a_position;
in vec2 a_texcoord;


out vec2 v_texcoord;
out vec3 v_position;
out float v_isPlane;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_position;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
    v_position = a_position.xyz;
    v_isPlane = isPlane;

}
//! [0]
