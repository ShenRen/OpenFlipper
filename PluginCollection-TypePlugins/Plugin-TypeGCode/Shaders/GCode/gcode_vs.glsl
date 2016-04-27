#version 330

in vec4 aPosition;
in vec4 aDx;
in vec4 aDy;
in vec4 aDz;
in vec4 aParams;

out vec4 vPosition;
out vec4 vDx;
out vec4 vDy;
out vec4 vDz;
out vec4 vParams;

uniform mat4 uProj;

void main(){
  vPosition = aPosition;
  vDx = aDx;
  vDy = aDy;
  vDz = aDz;
  vParams = aParams;

  gl_Position = uProj*aPosition;
}