#version 330

layout (lines) in;
layout (triangle_strip, max_vertices = 32) out;

uniform mat4 uProj;
uniform mat4 uView;
uniform mat3 uViewIT;
uniform float uTime;

in vec4 vPosition[];
in vec4 vDx[];
in vec4 vDy[];
in vec4 vDz[];
in vec4 vParams[];

out vec3 gNormal;
out float gEdgeType;
out float gTime;
out float gSpeed;

void emit_vertex(vec4 pos, vec4 dx, vec4 dy, vec4 normal){
       gl_Position = uProj*(vec4(pos.xyz, 1.0)+dx+dy);
       gNormal = normalize(uViewIT*normal.xyz);
       gSpeed = vParams[0].x;
       gEdgeType = vParams[0].y;
       gTime = pos[3];
       EmitVertex();
}

void emit_edge(vec4 pos_a, vec4 pos_b, vec4 dx_a, vec4 dy_a, vec4 dx_b, vec4 dy_b){
   const float scale = 1.0;
   
   emit_vertex(pos_a, dx_a, dy_a*scale, dx_a);
   emit_vertex(pos_b, dx_b, dy_b*scale, dx_b);
   
   emit_vertex(pos_a, dx_a, -dy_a*scale, dx_a);
   emit_vertex(pos_b, dx_b, -dy_b*scale, dx_b);
   EndPrimitive();
   
   emit_vertex(pos_a, dx_a*scale, -dy_a, -dy_a);
   emit_vertex(pos_b, dx_b*scale, -dy_b, -dy_b);
   
   emit_vertex(pos_a, -dx_a*scale, -dy_a, -dy_a);
   emit_vertex(pos_b, -dx_b*scale, -dy_b, -dy_b);
   EndPrimitive();
   
   emit_vertex(pos_a, -dx_a, -dy_a*scale, -dx_a);
   emit_vertex(pos_b, -dx_b, -dy_b*scale, -dx_b);
   
   emit_vertex(pos_a, -dx_a, dy_a*scale, -dx_a);
   emit_vertex(pos_b, -dx_b, dy_b*scale, -dx_b);
   EndPrimitive();
   
   emit_vertex(pos_a, -dx_a*scale, dy_a, dy_a);
   emit_vertex(pos_b, -dx_b*scale, dy_b, dy_a);
   
   emit_vertex(pos_a, dx_a*scale, dy_a, dy_a);
   emit_vertex(pos_b, dx_b*scale, dy_b, dy_a);
   EndPrimitive();
}

void emit_corner(vec4 pos_a, vec4 pos_b, vec4 dx_a, vec4 dy_a, vec4 dx_b, vec4 dy_b){
   const float scale = 1.0;
   
   emit_vertex(pos_b, dx_a, dy_a*scale, dx_a);
   emit_vertex(pos_b, dx_b, dy_b*scale, dx_b);
   
   emit_vertex(pos_b, dx_a, -dy_a*scale, dx_a);
   emit_vertex(pos_b, dx_b, -dy_b*scale, dx_b);
   EndPrimitive();
   
   emit_vertex(pos_b, dx_a*scale, -dy_a, -dy_a);
   emit_vertex(pos_b, dx_b*scale, -dy_b, -dy_b);
   
   emit_vertex(pos_b, -dx_a*scale, -dy_a, -dy_a);
   emit_vertex(pos_b, -dx_b*scale, -dy_b, -dy_b);
   EndPrimitive();
   
   emit_vertex(pos_b, -dx_a, -dy_a*scale, -dx_a);
   emit_vertex(pos_b, -dx_b, -dy_b*scale, -dx_b);
   
   emit_vertex(pos_b, -dx_a, dy_a*scale, -dx_a);
   emit_vertex(pos_b, -dx_b, dy_b*scale, -dx_b);
   EndPrimitive();
   
   emit_vertex(pos_b, -dx_a*scale, dy_a, dy_a);
   emit_vertex(pos_b, -dx_b*scale, dy_b, dy_a);
   
   emit_vertex(pos_b, dx_a*scale, dy_a, dy_a);
   emit_vertex(pos_b, dx_b*scale, dy_b, dy_a);
   EndPrimitive();
}

void main(){
  float start_time = vPosition[0].w;
  float end_time = vPosition[1].w;
  int edge_type = int(round(vParams[0].y));
  if(edge_type == 2){
    return;
  }
  if(end_time <= uTime){
       emit_edge(vPosition[0], vPosition[1], vec4(vDx[0].xyz*vDx[0].w, 0.0), vec4(vDy[0].xyz*vDy[0].w, 0.0), vec4(vDx[0].xyz*vDx[0].w, 0.0), vec4(vDy[0].xyz*vDy[0].w, 0.0));
       emit_corner(vPosition[0], vPosition[1], vec4(vDx[0].xyz*vDx[0].w, 0.0), vec4(vDy[0].xyz*vDy[0].w, 0.0), vec4(vDx[1].xyz*vDx[1].w, 0.0), vec4(vDy[1].xyz*vDy[1].w, 0.0));
  }else if(start_time <= uTime){
       float x = (uTime-start_time)/(end_time-start_time);
       vec4 position = vPosition[0]*(1.0-x) + vPosition[1]*x;
       
       emit_edge(vPosition[0], position, vec4(vDx[0].xyz*vDx[0].w, 0.0), vec4(vDy[0].xyz*vDy[0].w, 0.0), vec4(vDx[0].xyz*vDx[0].w, 0.0), vec4(vDy[0].xyz*vDy[0].w, 0.0));
  }
}