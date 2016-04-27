#version 330

uniform mat4 uProj;
uniform vec4 uHeadPosition;

void main(){
   gl_Position = uProj*uHeadPosition;
}