#version 150

uniform sampler2D texture_grass;
uniform sampler2D texture_rock;
uniform sampler2D texture_snow;

in float v_isPlane;
in vec2 v_texcoord;
in vec3 v_position;

//! [0]
void main()
{

    if(v_position.z <=3.0f)
        gl_FragColor = texture2D(texture_grass, v_texcoord);
    else if (v_position[2]>3.0f && v_position[2]<=4.0f)
        gl_FragColor = texture2D(texture_rock, v_texcoord);
    else
        gl_FragColor = texture2D(texture_snow, v_texcoord);


    if(v_isPlane ==0.0){
        gl_FragColor = vec4(0.0,0.0,0.8,1.0);
    }



}
//! [0]

