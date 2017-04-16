#version 150

// splat shader version compatible with the ACG shader generator

// request common attributes
#define SG_REQUEST_POSVS
#define SG_REQUEST_NORMALOS
#define SG_REQUEST_NORMALVS


out vec3 v2g_tangentVS;    // tangent in view space
out vec3 v2g_bitangentVS;  // bitangent in view space
out float v2g_pointSize; // point size in view space

uniform float pointsizeScale = 1.0;
uniform bool  backfaceCulling = false;
uniform float modelviewScale;
uniform float viewportScaleFov_y;
uniform float defaultPointsize;
uniform vec3  defaultNormal;

void main()
{
#ifdef SG_INPUT_TEXCOORD
  (SG_INPUT_TEXCOORD.x is the pointsize in model - coordinates)
    float splatPointSize = SG_INPUT_TEXCOORD.x;
#else
  float splatPointSize = defaultPointsize;
#endif


  // transformation of splat center is done in generated shader code:
  SG_VERTEX_BEGIN;

  SG_VERTEX_END;
  
      // compute normal-aligned tangent vectors

    if (all(lessThan(abs(sg_vNormalVS.xy), vec2(0.1, 0.1))))
      v2g_tangentVS = cross(sg_vNormalVS, vec3(0, 1, 0));
    else
      v2g_tangentVS = cross(sg_vNormalVS, vec3(0, 0, 1));

    v2g_tangentVS = normalize(v2g_tangentVS);
    v2g_bitangentVS = cross(sg_vNormalVS, v2g_tangentVS);


    // calculate pointsize (0.5 factor: scale to point radius instead of diameter)
    v2g_pointSize = modelviewScale * pointsizeScale * splatPointSize * 0.5;

  // use generated SG_OUTPUT variables to do backface culling
  if (backfaceCulling &&  dot(SG_OUTPUT_POSVS, SG_OUTPUT_NORMALVS)  > 0.0)
  {
    // discard vertex
    gl_Position.w = 0.0;
	SG_OUTPUT_POSVS.w = 0.0;
  }
}
