#version 130

// splat shader version compatible with the ACG shader generator

// request common attributes
#define SG_REQUEST_POSVS
#define SG_REQUEST_NORMALOS
#define SG_REQUEST_NORMALVS


uniform float pointsizeScale  = 1.0;
uniform bool  backfaceCulling = false;
uniform float modelviewScale;
uniform float viewportScaleFov_y;
uniform float defaultPointsize;
uniform vec3  defaultNormal;

void main()
{
  // transformation of splat center is done in generated shader code:
  SG_VERTEX_BEGIN;

  SG_VERTEX_END;

  // use generated SG_OUTPUT variables to do backface culling
  if (backfaceCulling &&  dot(SG_OUTPUT_POSVS, SG_OUTPUT_NORMALVS)  > 0.0)
  {
    // discard vertex
    gl_Position.w = 0.0;
  }
}
