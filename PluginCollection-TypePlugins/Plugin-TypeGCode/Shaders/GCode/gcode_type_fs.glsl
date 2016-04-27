#version 330

in vec3 gNormal;
in float gEdgeType;
in float gTime;
in float gSpeed;

out vec4 oColor;

uniform float uTime;

void main(){
  int edge_type = int(round(gEdgeType));
  vec3 color = vec3(0.0, 0.0, 0.0);
  if(edge_type == 0){
     color = vec3(0.0, 0.0, 1.0);
  }else if(edge_type == 1){
     color = vec3(0.0, 1.0, 0.0);
  }else if(edge_type == 2){
     color = vec3(1.0, 0.0, 0.0);
  }else if(edge_type == 3){
     color = vec3(0.0, 1.0, 1.0);
  }else if(edge_type == 4){
     color = vec3(1.0, 0.0, 1.0);
  }
  
  vec3 normal = normalize(gNormal);
  oColor = vec4(color, 1.0)*max(normal[2], 0.0);
}