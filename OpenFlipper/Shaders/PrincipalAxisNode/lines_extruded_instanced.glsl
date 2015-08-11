#version 140
#extension GL_EXT_gpu_shader4 : enable
// extension for integer modulo

in float pa_xOffset;

in vec4 pa_worldTransform0;
in vec4 pa_worldTransform1;
in vec4 pa_worldTransform2;

in float pa_size;


uniform float pa_lineWidth;
uniform vec2 pa_screenSize;

uniform vec3 pa_scale;
uniform vec3 pa_offset;

void main()
{
  SG_VERTEX_BEGIN;
  
  int pa_component = gl_InstanceID % 3;
  
  // positions of the line end-points
  vec4 pa_posOS[2];
  pa_posOS[0] = SG_INPUT_POSOS;
  pa_posOS[1] = SG_INPUT_POSOS;
  pa_posOS[1].z = 1.0 - pa_posOS[1].z;
  
  // apply scale + translation if visualized in both directions
  pa_posOS[0].z = pa_posOS[0].z * pa_scale[pa_component] + pa_offset[pa_component];
  pa_posOS[1].z = pa_posOS[1].z * pa_scale[pa_component] + pa_offset[pa_component];
  
  pa_posOS[0].z *= pa_size;
  pa_posOS[1].z *= pa_size;
  
  // transform to world space
  vec4 pa_posWS[2];
  
  for (int i = 0; i < 2; ++i)
  {
    pa_posWS[i].x = dot(pa_worldTransform0, pa_posOS[i]);
    pa_posWS[i].y = dot(pa_worldTransform1, pa_posOS[i]);
    pa_posWS[i].z = dot(pa_worldTransform2, pa_posOS[i]);
    pa_posWS[i].w = 1.0;
  }
  
  // transform to clip space
  vec4 pa_posCS[2];
  pa_posCS[0] = g_mWVP * pa_posWS[0];
  pa_posCS[1] = g_mWVP * pa_posWS[1];
  
  
  // quad extrusion
  
  // convert to screen space
  vec2 pa_posSS[2];
  pa_posSS[0].xy = pa_posCS[0].xy / pa_posCS[0].w * pa_screenSize;
  pa_posSS[1].xy = pa_posCS[1].xy / pa_posCS[1].w * pa_screenSize;

  // compute dir and normal
  vec2 pa_lineDir = pa_posSS[1].xy - pa_posSS[0].xy;
  vec2 pa_lineNormal = normalize(vec2(-pa_lineDir.y, pa_lineDir.x));

  // offsetting
  vec2 pa_offset = pa_lineNormal * (pa_lineWidth * pa_xOffset);

  sg_vPosPS = vec4( (pa_posSS[0].xy + pa_offset) * pa_posCS[0].w / pa_screenSize, pa_posCS[0].z, pa_posCS[0].w);
  
  
  SG_VERTEX_END;
}