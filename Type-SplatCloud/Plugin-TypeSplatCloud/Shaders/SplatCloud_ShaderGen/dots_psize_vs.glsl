#version 130

// splat shader version compatible with the ACG shader generator

// request common attributes
#define SG_REQUEST_POSVS
#define SG_REQUEST_NORMALVS


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
  
  // calculate pointsize
  float ecPointsize  = modelviewScale * pointsizeScale * splatPointSize;
  gl_PointSize = max(viewportScaleFov_y * ecPointsize  / gl_Position.w, 1.0);

  // use generated SG_OUTPUT variables to do backface culling
  if (backfaceCulling &&  dot(SG_OUTPUT_POSVS, SG_OUTPUT_NORMALVS)  > 0.0)
  {
    // discard vertex
    gl_Position.w = 0.0;
  }
}
