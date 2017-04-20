#version 130

// splat shader version compatible with the ACG shader generator

// request common attributes
#define SG_REQUEST_POSVS
#define SG_REQUEST_NORMALOS
#define SG_REQUEST_NORMALVS


out vec3 scaledNormalVS;
out float squaredRadiusVS;

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




  // pass normal in eye-coordinates to fragment-shader
  //   - scale so we do not have to do this in fragment-shader for every fragment
  scaledNormalVS = SG_OUTPUT_NORMALVS / dot(SG_OUTPUT_POSVS.xyz, SG_OUTPUT_NORMALVS);

  // calculate pointsize in view space
  float pointsizeVS = modelviewScale * pointsizeScale * splatPointSize;


  // pass squared radius (= (1/2 pointsize)^2) in eye-coordinates to fragment-shader
  squaredRadiusVS = 0.25 * (pointsizeVS * pointsizeVS);


  // output pointsize in window-coordinates
  //   - divided by gl_Position.w to shrink size by distance
  //   - divided by -normalize(posVS).z because splat could be oriented towards the eye (not necessarily parallel to near-plane)
  //   - multiply by viewportScaleFov_y to get window coordinates

  gl_PointSize = max(pointsizeVS * viewportScaleFov_y / (-normalize(SG_OUTPUT_POSVS.xyz).z * gl_Position.w), 1.0);


  // use generated SG_OUTPUT variables to do backface culling
  if (backfaceCulling &&  dot(SG_OUTPUT_POSVS, SG_OUTPUT_NORMALVS)  > 0.0)
  {
    // discard vertex
    gl_Position.w = 0.0;
  }
}
