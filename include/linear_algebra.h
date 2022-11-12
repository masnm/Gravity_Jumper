#ifndef __LINEAR_ALGEBRA_H__
#define __LINEAR_ALGEBRA_H__

typedef struct {
	int x;
	int y;
} vec2i ;

typedef struct {
	long x;
	long y;
} vec2l ;

typedef struct {
	float x;
	float y;
} vec2f ;

typedef struct {
	double x;
	double y;
} vec2d ;

// makeing "objs"
vec2i make_vec2i ( int _x, int _y );
vec2f make_vec2f ( float _x, float _y );
vec2d make_vec2d ( double _x, double _y );
vec2l make_vec2l ( long _x, long _y );

// operation add mul sub div
vec2i add_vec2i ( vec2i l, vec2i r );
vec2i sub_vec2i ( vec2i l, vec2i r );
vec2f add_vec2f ( vec2f l, vec2f r );
vec2f sub_vec2f ( vec2f l, vec2f r );
vec2d add_vec2d ( vec2d l, vec2d r );
vec2d sub_vec2d ( vec2d l, vec2d r );
vec2l add_vec2l ( vec2l l, vec2l r );
vec2l sub_vec2l ( vec2l l, vec2l r );

// vector to scaler
vec2i div_vec2i_scl ( vec2i l, int scl );
vec2d div_vec2d_scl ( vec2d l, double scl );

#endif // __LINEAR_ALGEBRA_H__
