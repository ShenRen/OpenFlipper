// shader for rendering the interpolation of the currently active segment
//  (current time is within this segment)

out vec4 v2f_params;

uniform vec4 uParams;

void main()
{
  SG_VERTEX_BEGIN;
  SG_VERTEX_END;
  
  v2f_params = uParams;
}