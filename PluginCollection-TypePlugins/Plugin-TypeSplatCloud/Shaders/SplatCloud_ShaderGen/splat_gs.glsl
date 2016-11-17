#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;


in vec3 v2g_tangentVS[];
in vec3 v2g_bitangentVS[];
in float v2g_pointSize[];

out vec2 g2f_posQS; // quad space position in range [-1,1]^2

uniform vec2 screenSize;


void main()
{
  // center of point
  vec4 centerVS = SG_INPUT_POSVS[0];
  
  // point size
  float pointSize = v2g_pointSize[0].x;
  
  // create normal-aligned quad
  vec3 offsetX = v2g_tangentVS[0] * pointSize;
  vec3 offsetY = v2g_bitangentVS[0] * pointSize;

  // offset factors for the quad vertices
  vec2 posQS[4] =
  {
    vec2(-1, 1),
	vec2(-1, -1),
	vec2(1, 1),
	vec2(1, -1)
  };
  
  for (int i = 0; i < 4; ++i)
  {
    // pass default io from shader generator
    sg_MapIO(i);
	
	vec2 uv = posQS[i];
	
	// expand to quad
	vec4 posVS = centerVS;
	posVS.xyz += uv.x * offsetX + uv.y * offsetY;
	
	// transform
	vec4 posCS = g_mP * posVS;
	
	// output
	gl_Position = posCS;
	g2f_posQS = uv;
	
    #ifdef SG_OUTPUT_POSVS
     SG_OUTPUT_POSVS = posVS;
    #endif
	
	#ifdef SG_OUTPUT_POSCS
	 SG_OUTPUT_POSCS = posCS;
	#endif
	
	EmitVertex();
  }
  
  EndPrimitive();
}
