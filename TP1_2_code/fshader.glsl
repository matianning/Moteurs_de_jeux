#version 150

uniform sampler2D texture;
uniform sampler2D texture_grass;
uniform sampler2D texture_rock;
uniform sampler2D texture_snow;

in vec2 v_texcoord;
in vec3 v_position;
//! [0]
void main()
{
    // Set fragment color from texture
    //gl_FragColor = texture2D(texture, v_texcoord);

    if(v_position.z <=0.1f)
        gl_FragColor = texture2D(texture_grass, v_texcoord);
    else if (v_position[2]>0.1f && v_position[2]<=0.3f)
        gl_FragColor = texture2D(texture_rock, v_texcoord);
    else
        gl_FragColor = texture2D(texture_snow, v_texcoord);



}
//! [0]

