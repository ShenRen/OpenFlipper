#version 140
#extension GL_EXT_gpu_shader4 : enable
// extension for integer modulo

in vec4 pa_worldTransform0;
in vec4 pa_worldTransform1;
in vec4 pa_worldTransform2;

in float pa_size;


uniform float pa_cone_radius;

uniform vec3 pa_scale;
uniform vec3 pa_offset;

void main()
{
  SG_VERTEX_BEGIN;
  
  int pa_component = gl_InstanceID % 3;
  
  vec4 pa_posOS = SG_INPUT_POSOS;
  
  pa_posOS.xy *= pa_cone_radius;
  pa_posOS.z = pa_posOS.z * pa_scale[pa_component] + pa_offset[pa_component];
  pa_posOS.z *= pa_size;
  
  vec4 pa_posWS;
  pa_posWS.x = dot(pa_worldTransform0, pa_posOS);
  pa_posWS.y = dot(pa_worldTransform1, pa_posOS);
  pa_posWS.z = dot(pa_worldTransform2, pa_posOS);
  pa_posWS.w = 1.0;
  
#ifdef SG_INPUT_NORMALOS
  sg_vNormalVS.x = dot(pa_worldTransform0.xyz, SG_INPUT_NORMALOS.xyz);
  sg_vNormalVS.y = dot(pa_worldTransform1.xyz, SG_INPUT_NORMALOS.xyz);
  sg_vNormalVS.z = dot(pa_worldTransform2.xyz, SG_INPUT_NORMALOS.xyz);

  sg_vNormalVS = g_mWVIT * sg_vNormalVS;
  sg_vNormalVS = normalize(sg_vNormalVS);
#endif
  
  sg_vPosVS = g_mWV * pa_posWS;
  sg_vPosPS = g_mWVP * pa_posWS;
  
  SG_VERTEX_END;
}