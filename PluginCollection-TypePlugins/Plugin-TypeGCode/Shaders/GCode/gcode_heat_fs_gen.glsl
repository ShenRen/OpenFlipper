#define SG_REQUEST_NORMALVS

in vec4 v2f_params;

uniform float uTime;

void main()
{
  SG_FRAGMENT_BEGIN;
  SG_FRAGMENT_END;
  
  float time_passed = (uTime - v2f_params.x);
  float heat = exp(-time_passed/10.0);
  heat = clamp(heat, 0, 1);
  outFragment *= vec4(heat, 1.0-heat, 0.0, 1.0);
}