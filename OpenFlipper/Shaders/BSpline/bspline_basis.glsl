
// preprocessor macros
//  BSPLINE_DEGREE_U
//  BSPLINE_DEGREE_V
// should be defined


#ifndef BSPLINE_DEGREE_U
#define BSPLINE_DEGREE_U 4
#endif

#ifndef BSPLINE_DEGREE_V
#define BSPLINE_DEGREE_V 4
#endif


// linear search
int bspline_find_span_ls(float u, samplerBuffer knots, int degree)
{
  int i = 0;
  
  int n = textureSize(knots);
  
  float upperBound = texelFetch(knots, n - 1).x;
  
  
  if (u >= upperBound)
    return n - degree - 2;
  
  float x = texelFetch(knots, i).x;
  
  while (u >= x && i < n)
  {
    ++i;
    x = texelFetch(knots, i).x;
  }
  
  while (u < x && i >= 0)
  {
    --i;
	x = texelFetch(knots, i).x;
  }
  
  return i;
}

// binary search
int bspline_find_span_bs(float u, samplerBuffer knots, int degree)
{
  // binary search
  
  // broken -> inf loop
  
  int lo = degree;
  int hi = textureSize(knots) - 1 - degree;
  
  float upperBound = texelFetch(knots, hi).x;
  
  if (u >= upperBound)
    return hi - 1;
  
  int mid = (lo + hi) / 2;
  
  float knot_mid = texelFetch(knots, mid).x;
  float knot_mid1 = texelFetch(knots, mid + 1).x;
  
  while (u < knot_mid || u >= knot_mid1)
  {
    if (u < knot_mid)
	  hi = mid;
	else
	  lo = mid;
	  
	mid = (lo + hi) / 2;
	
	knot_mid = texelFetch(knots, mid).x;
	knot_mid1 = texelFetch(knots, mid + 1).x;
  }
  
  return mid;
}


void bspline_derivs_U(out float N[BSPLINE_DEGREE_U+1], out float ND[BSPLINE_DEGREE_U+1], const int i, float u, samplerBuffer knots)
{
  const int p = BSPLINE_DEGREE_U;
  const int p1 = p+1;
  
  vec2 lr[p1];
  N[0] = 1;
  
  for (int j = 1; j <= p; ++j)
  {
    lr[j].x = u - texelFetch(knots, i + 1 - j).x;
	lr[j].y = texelFetch(knots, i + j).x - u;
	
	float saved = 0;
	
	for (int r = 0; r < j; ++r)
	{
	  float left = lr[j-r].x;
	  float right = lr[r+1].y;
	  
	  float tmp = N[r] / (left + right);
	  N[r] = saved + right * tmp;
	  saved = left * tmp;
	}
	
	N[j] = saved;
	
	if (j == p - 1)
	{
	  // N currently stores N(j, p-1), so compute derivatives now
	  // The NURBS Book p59 eq 2.7
	  
	  // dN(i,p) / du = (p / (knot[i+p] - knot[i])) * N(i,p-1) - (p / (knot[i + p + 1] - knot[i + 1]) * N(i+1,p-1)
	  // evaluate this efficiently not only for i, but for all j = i-p, .., i
	  
	  // left first N term is 0
	  float lterm = 0;
	  
	  for (int r = 0; r < p; ++r)
	  {
	    // terms in in eq 2.7:
		// index i: i - p + r
	    // lterm:  p * N(i, p-1) / (knots[i+p] - knots[i])   (left term)
	    float kp1 = texelFetch(knots, i + 1 + r).x;      // knots[i+p+1]
		float kp2 = texelFetch(knots, i + 1 - p + r).x;  // knots[i+1]
		float rterm = float(p) * N[r] / (kp1 - kp2);     // p * N(i+1, p-1)/ (kp1 - kp2)
		
		ND[r] = lterm - rterm;
		
		// right term is on the left for next N'(r+1, p)
		lterm = rterm;
	  }
	  
	  // right last N term is 0, only left term remains
	  ND[p] = lterm;  
	}
  }
}

void bspline_derivs_V(out float N[BSPLINE_DEGREE_V+1], out float ND[BSPLINE_DEGREE_V+1], const int i, float u, samplerBuffer knots)
{
  const int p = BSPLINE_DEGREE_V;
  const int p1 = p+1;
  
  vec2 lr[p1];
  N[0] = 1;
  
  for (int j = 1; j <= p; ++j)
  {
    lr[j].x = u - texelFetch(knots, i + 1 - j).x;
	lr[j].y = texelFetch(knots, i + j).x - u;
	
	float saved = 0;
	
	for (int r = 0; r < j; ++r)
	{
	  float left = lr[j-r].x;
	  float right = lr[r+1].y;
	  
	  float tmp = N[r] / (left + right);
	  N[r] = saved + right * tmp;
	  saved = left * tmp;
	}
	
	N[j] = saved;
	
	if (j == p - 1)
	{
	  // N currently stores N(j, p-1), so compute derivatives now
	  
	  saved = 0;
	  
	  for (int r = 0; r < p; ++r)
	  {
	    float kp1 = texelFetch(knots, i + 1 + r).x;
		float kp2 = texelFetch(knots, i + 1 - p + r).x;
		float fac2 = float(p) * N[r] / (kp1 - kp2);
		
		ND[r] = saved - fac2;
		
		saved = fac2;
	  }
	  
	  ND[p] = saved;
	}
  }
}