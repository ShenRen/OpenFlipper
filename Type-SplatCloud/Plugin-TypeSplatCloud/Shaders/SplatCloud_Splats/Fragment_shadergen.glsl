#version 140

// splat shader version compatible with the ACG shader generator

in vec3  ecScaledNormal;
in float ecSquaredRadius;

uniform mat4  invProjection;

uniform vec4  invViewportScale;
uniform vec4  invViewportTransp;
uniform float viewportScale_z;
uniform float viewportTransp_z;

void main()
{
    SG_FRAGMENT_BEGIN;
	
    // calculate fragment's position in eye-coordinates
    //   - scaled by w-component (not divided) because the fragment's projection (see below) is invariant to this
    vec4 ndcPosition      = invViewportScale * gl_FragCoord + invViewportTransp;
    vec3 ecScaledPosition = vec3( invProjection * ndcPosition );

	
	vec3 ecCenter = SG_INPUT_POSVS.xyz;
	
    // calculate projection in eye-coordinates of fragment's position along viewing direction onto plane described by splat's center and normal
    //   - equivalent to: ecProjection = (dot(ecCenter,ecNormal) / dot(ecPosition,ecNormal)) * ecPosition
    vec3 ecProjection = ecScaledPosition / dot( ecScaledPosition, ecScaledNormal );

    // discard fragment if projection is too far away from splat's center
    vec3 ecCenterToProj = ecProjection - ecCenter;
    if( dot( ecCenterToProj, ecCenterToProj ) > ecSquaredRadius )
        discard;

    // calculate depth of projection in normalized-device-coordinates
    vec4  ccProjection    = g_mP * vec4( ecProjection, 1.0 );
    float ndcProjection_z = ccProjection.z / ccProjection.w;

	SG_FRAGMENT_END;
	
    // output depth of projection in window-coordinates
    gl_FragDepth = viewportScale_z * ndcProjection_z + viewportTransp_z;
}
