#include "../include/linear_algebra.h"

// makeing "objs"
vec2i make_vec2i ( int _x, int _y )
{
	return (vec2i) {
		.x = _x,
		.y = _y
	};
}

vec2f make_vec2f ( float _x, float _y )
{
	return (vec2f) {
		.x = _x,
		.y = _y
	};
}

vec2d make_vec2d ( double _x, double _y )
{
	return (vec2d) {
		.x = _x,
		.y = _y
	};
}

vec2l make_vec2l ( long _x, long _y )
{
	return (vec2l) {
		.x = _x,
		.y = _y
	};
}

// operation add mul sub div
vec2i add_vec2i ( vec2i l, vec2i r )
{
	return (vec2i) {
		.x = l.x + r.x,
		.y = l.y + r.y
	};
}

vec2f add_vec2f ( vec2f l, vec2f r )
{
	return (vec2f) {
		.x = l.x + r.x,
		.y = l.y + r.y
	};
}

vec2i sub_vec2i ( vec2i l, vec2i r )
{
	return (vec2i) {
		.x = l.x - r.x,
		.y = l.y - r.y
	};
}

vec2f sub_vec2f ( vec2f l, vec2f r )
{
	return (vec2f) {
		.x = l.x - r.x,
		.y = l.y - r.y
	};
}

vec2d add_vec2d ( vec2d l, vec2d r )
{
	return (vec2d) {
		.x = l.x + r.x,
		.y = l.y + r.y
	};
}

vec2d sub_vec2d ( vec2d l, vec2d r )
{
	return (vec2d) {
		.x = l.x - r.x,
		.y = l.y - r.y
	};
}

vec2l add_vec2l ( vec2l l, vec2l r )
{
	return (vec2l) {
		.x = l.x + r.x,
		.y = l.y + r.y
	};
}

vec2l sub_vec2l ( vec2l l, vec2l r )
{
	return (vec2l) {
		.x = l.x - r.x,
		.y = l.y - r.y
	};
}

vec2i div_vec2i_scl ( vec2i l, int scl )
{
	return (vec2i) {
		.x = l.x / scl,
		.y = l.y / scl
	};
}

vec2d div_vec2d_scl ( vec2d l, double scl )
{
	return (vec2d) {
		.x = l.x / scl,
		.y = l.y / scl
	};
}
