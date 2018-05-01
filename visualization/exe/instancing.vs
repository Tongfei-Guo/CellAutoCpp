#version 330

in vec4 position;
in vec4 instance_color;
in vec4 instance_position;

uniform ivec2 gridsize;  //(columnsize, rowsize)

out Fragment
{
    vec4 color;
} fragment;

void main(void)
{
    vec2 sqsize = vec2(2,2)/gridsize;  
    ivec2 sqind = ivec2( mod(gl_InstanceID,gridsize[0]), round(gl_InstanceID/gridsize[0]));      //(column,row)
    vec2 pos2d = vec2(-1.0,-1.0) + sqsize/2 + sqsize*sqind;    
    gl_Position = position*vec4(sqsize,1,1) + vec4(pos2d,0,0);
    //gl_Position = position + vec4(gl_InstanceID*0.2,0,0,0); //+ instance_position ;//* vec4(0.1, 0.1, 1.0, 1.0);
    fragment.color = instance_color;
}
