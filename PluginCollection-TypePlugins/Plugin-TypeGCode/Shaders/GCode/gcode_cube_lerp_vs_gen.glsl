// shader for rendering the interpolation of the currently active segment
//  (current time is within this segment)

out float gEdgeType;
out float gTime;
out float gSpeed;

uniform vec4 uParams;

void main()
{
  SG_VERTEX_BEGIN;
  SG_VERTEX_END;
  
  gEdgeType = uParams.w;
  gTime = uParams.x;
  gSpeed = uParams.z;
}