#version 140

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


out vec3  ecScaledNormal;
out float ecSquaredRadius;

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
		
        // pass splat's center in eye-coordinates to fragment-shader
        vec3 ecCenter = sg_vPosVS.xyz;

        // pass normal in eye-coordinates to fragment-shader
        //   - scale so we do not have to do this in fragment-shader for every fragment
        ecScaledNormal = sg_vNormalVS / dot( ecCenter, sg_vNormalVS );

        // calculate pointsize (SG_INPUT_TEXCOORD.x is the pointsize in model-coordinates)
        float ecPointsize  = modelviewScale * pointsizeScale * splatPointSize;
		
        // pass squared radius (= (1/2 pointsize)^2) in eye-coordinates to fragment-shader
        ecSquaredRadius = 0.25 * (ecPointsize * ecPointsize);

        // output pointsize in window-coordinates
        //   - divided by gl_Position.w to shrink size by distance
        //   - divided by -normalize(ecCenter).z because splat could be oriented towards the eye (not necessarily parallel to near-plane)
        //   - multiply by viewportScaleFov_y to get window coordinates
        gl_PointSize = max( ecPointsize * viewportScaleFov_y / (-normalize( ecCenter ).z * gl_Position.w), 1.0 );


		SG_VERTEX_END;
	}
}
