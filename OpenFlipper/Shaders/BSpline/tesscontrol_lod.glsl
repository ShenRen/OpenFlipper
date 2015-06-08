#version 400

#define SG_REQUEST_POSVS

layout(vertices = 3) out;


void main()
{
  sg_MapIO(gl_InvocationID);
  
  
  // lod based on distance to viewer
  vec3 posVS = SG_INPUT_POSVS[gl_InvocationID].xyz;
  float d2 = dot(posVS, posVS);
  
  float d = sqrt(d2);
  
  // sq, lin, const
  vec3 atten = vec3(0.001, 0.08, 0.3);
  
  float lod = 1.0 / (dot(atten, vec3(d2, d, 1.0)));
  
  if (gl_InvocationID == 0)
  {
    float lod1 = lod, lod2 = lod;
    gl_TessLevelInner[0] = lod1;
	gl_TessLevelInner[1] = lod1;
    gl_TessLevelOuter[0] = lod2;
	gl_TessLevelOuter[1] = lod2;
	gl_TessLevelOuter[2] = lod2;
	gl_TessLevelOuter[3] = lod2;
  }
}