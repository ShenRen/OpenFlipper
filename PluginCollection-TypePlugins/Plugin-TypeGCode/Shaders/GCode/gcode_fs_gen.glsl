#define SG_REQUEST_NORMALVS

in float gEdgeType;
in float gTime;
in float gSpeed;

uniform float uTime;

void main()
{
  SG_FRAGMENT_BEGIN;
  SG_FRAGMENT_END;
  
  float time_passed = (uTime - gTime);
  float heat = exp(-time_passed/10000.0);
  outFragment *= vec4(heat, 1.0-heat, 0.0, 1.0);
}