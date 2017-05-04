#version 130

// splat shader version compatible with the ACG shader generator

in vec3  scaledNormalVS;
in float squaredRadiusVS;

uniform vec4  invViewportScale;
uniform vec4  invViewportTransp;
uniform float viewportScale_z;
uniform float viewportTransp_z;
uniform mat4  invProjection;

void main()
{
  SG_FRAGMENT_BEGIN;
  SG_FRAGMENT_END;

  // calculate fragment's position in eye-coordinates
  //   - scaled by w-component (not divided) because the fragment's projection (see below) is invariant to this
  vec4 ndcPosition = invViewportScale * gl_FragCoord + invViewportTransp;
  vec3 scaledPosVS = vec3(invProjection * ndcPosition);

  // calculate projection in eye-coordinates of fragment's position along viewing direction onto plane described by splat's center and normal
  //   - equivalent to: projectionVS = (dot(centerVS,normalVS) / dot(fragmentVS,normalVS)) * fragmentVS
  vec3 projectionVS = scaledPosVS / dot(scaledPosVS, scaledNormalVS);

  // discard fragment if projection is too far away from splat's center
  vec3 centerToProjVS = projectionVS - SG_INPUT_POSVS.xyz;
  if (dot(centerToProjVS, centerToProjVS) > squaredRadiusVS)
    discard;

  // calculate depth of projection in normalized-device-coordinates
  vec4  projectionCS = g_mP * vec4(projectionVS, 1.0);
  float ndcProjection_z = projectionCS.z / projectionCS.w;

  // output depth of projection in window-coordinates
  gl_FragDepth = viewportScale_z * ndcProjection_z + viewportTransp_z;
}
