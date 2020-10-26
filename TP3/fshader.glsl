#version 150

uniform sampler2D texture_grass;
uniform sampler2D texture_rock;
uniform sampler2D texture_snow;

in vec2 v_texcoord;
in vec3 v_position;

out vec4 gl_FragColor;
//! [0]
void main()
{
    /*
    if(v_position.z <=0.5f)
        gl_FragColor = texture2D(texture_grass, v_texcoord);
    else if (v_position[2]>0.5f && v_position[2]<=2.5f)
        gl_FragColor = texture2D(texture_rock, v_texcoord);
    else
        gl_FragColor = texture2D(texture_snow, v_texcoord);
*/
    gl_FragColor = vec4(0.0,0.0,1.0,1.0);


}
//! [0]

