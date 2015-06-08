#version 400

#define SG_REQUEST_TEXCOORD
#define SG_REQUEST_NORMALVS

layout(triangles, equal_spacing, ccw) in;


uniform samplerBuffer knotBufferU;
uniform samplerBuffer knotBufferV;

uniform sampler2D controlPointTex;

uniform vec4 uvRange;


#include "bspline_basis.glsl"


void main()
{
//  sg_MapIOBilinear();
  sg_MapIOBarycentric();
  

  vec2 uv = SG_OUTPUT_TEXCOORD;

  // map [0,1] to actual parameter range
  uv.x = mix(uvRange.x, uvRange.y, uv.x);
  uv.y = mix(uvRange.z, uvRange.w, uv.y);
  

  // find span index (i) in u and v dir
  int span_u = bspline_find_span_bs(uv.x, knotBufferU, BSPLINE_DEGREE_U);
  int span_v = bspline_find_span_bs(uv.y, knotBufferV, BSPLINE_DEGREE_V);

  float basisFuncsU[BSPLINE_DEGREE_U+1];
  float basisFuncsV[BSPLINE_DEGREE_V+1];
  
  float basisFuncsDU[BSPLINE_DEGREE_U+1];
  float basisFuncsDV[BSPLINE_DEGREE_V+1];

  bspline_derivs_U(basisFuncsU, basisFuncsDU, span_u, uv.x, knotBufferU);
  bspline_derivs_V(basisFuncsV, basisFuncsDV, span_v, uv.y, knotBufferV);
  
  // position
  vec3 pos = vec3(0,0,0);
  
  // tangents
  vec3 dpdu = vec3(0,0,0); // derivative: d pos / du
  vec3 dpdv = vec3(0,0,0); // derivative: d pos / dv

  // linear combination
  for (int x = 0; x <= BSPLINE_DEGREE_U; ++x)
  {
    for (int y = 0; y <= BSPLINE_DEGREE_V; ++y)
	{
	  // load control point
	  ivec2 controlPtID = ivec2(span_u - BSPLINE_DEGREE_U + x, span_v - BSPLINE_DEGREE_V + y);
	  vec3 controlPt = texelFetch(controlPointTex, controlPtID, 0).xyz;
	  
	  // combine
	  pos += controlPt * (basisFuncsU[x] * basisFuncsV[y]);
	  dpdu += controlPt * (basisFuncsDU[x] * basisFuncsV[y]);
	  dpdv += controlPt * (basisFuncsU[x] * basisFuncsDV[y]);
	}
  }
  
  vec3 surfaceNormal = normalize(cross(dpdu, dpdv));

  gl_Position = g_mWVP * vec4(pos, 1);

#ifdef SG_OUTPUT_POSOS
  SG_OUTPUT_POSOS = vec4(pos, 1);
#endif
#ifdef SG_OUTPUT_POSVS
  SG_OUTPUT_POSVS = g_mWV * vec4(pos, 1);
#endif
#ifdef SG_OUTPUT_POSCS
  SG_OUTPUT_POSCS = gl_Position;
#endif

#ifdef SG_OUTPUT_NORMALOS
  SG_OUTPUT_NORMALOS = surfaceNormal;
#endif
#ifdef SG_OUTPUT_NORMALVS
  SG_OUTPUT_NORMALVS = g_mWVIT * surfaceNormal;
#endif


}