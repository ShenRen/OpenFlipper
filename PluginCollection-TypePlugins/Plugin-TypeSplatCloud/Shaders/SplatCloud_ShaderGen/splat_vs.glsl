#version 150

////////////////////////////////////////////////////////////////////////////////////////////
/**/                                                                                    /**/
/**/  // WARNING: When modifying this file, modify file "PickVertex.glsl" accordingly!  /**/
/**/                                                                                    /**/
////////////////////////////////////////////////////////////////////////////////////////////


// splat shader version compatible with the ACG shader generator

// request common attributes
#define SG_REQUEST_POSVS
#define SG_REQUEST_NORMALOS
#define SG_REQUEST_NORMALVS


out vec3 v2g_tangentVS;    // tangent in view space
out vec3 v2g_bitangentVS;  // bitangent in view space
out float v2g_pointSize; // point size in view space

uniform float pointsizeScale  = 1.0;
uniform bool  backfaceCulling = false;
uniform float modelviewScale;
uniform float viewportScaleFov_y;
uniform float defaultPointsize;
uniform vec3  defaultNormal;

void main()
{
#ifdef SG_INPUT_NORMALOS
    vec3 splatNormal = SG_INPUT_NORMALOS;
#else
    vec3 splatNormal = defaultNormal;
#endif

#ifdef SG_INPUT_TEXCOORD
    (SG_INPUT_TEXCOORD.x is the pointsize in model-coordinates)
    float splatPointSize = SG_INPUT_TEXCOORD.x;
#else
    float splatPointSize = defaultPointsize;
#endif

	
    // cull if normal is pointing away from eye
    if( backfaceCulling && dot( SG_INPUT_POSOS.xyz - g_vCamPos, splatNormal ) > 0.0 ) // second part is evaluated only if backfaceCulling is true
    {
        // discard vertex
        gl_Position.w = 0.0;
    }
    else
    {
		SG_VERTEX_BEGIN;
		
		// compute tangent vectors
		
		if (all(lessThan(abs(sg_vNormalVS.xy), vec2(0.1, 0.1))))
		  v2g_tangentVS = cross(sg_vNormalVS, vec3(0,1,0));
		else
		  v2g_tangentVS = cross(sg_vNormalVS, vec3(0,0,1));
		  
		v2g_tangentVS = normalize(v2g_tangentVS);
		v2g_bitangentVS = cross(sg_vNormalVS, v2g_tangentVS);
		
        
        // calculate pointsize
        v2g_pointSize  = modelviewScale * pointsizeScale * splatPointSize;
		
		SG_VERTEX_END;
	}
}
