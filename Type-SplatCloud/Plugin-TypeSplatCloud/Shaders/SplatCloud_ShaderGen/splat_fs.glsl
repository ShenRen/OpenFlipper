#version 150

// splat shader version compatible with the ACG shader generator

in vec2 g2f_posQS;

void main()
{
  // discard fragments outside of splat radius
  if( dot( g2f_posQS, g2f_posQS ) > 1 )
    discard;

  SG_FRAGMENT_BEGIN;
  SG_FRAGMENT_END;
}
