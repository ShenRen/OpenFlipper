#define SG_REQUEST_NORMALVS

in vec4 v2f_params;

uniform float uTime;

void main()
{
  SG_FRAGMENT_BEGIN;
  SG_FRAGMENT_END;
  
  float speed = min((v2f_params.z-1200.0)/(4800.0-1200.0), 1.0);
  outFragment *= vec4(1.0-speed, speed, 0.0, 1.0);
}