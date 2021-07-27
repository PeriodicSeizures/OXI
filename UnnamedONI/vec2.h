// common types
#ifndef __VEC2_H__
#define __VEC2_H__

#include <cmath>

#include "lib/gl.h"

class vec2 {
public:
	GLFix x, y;
	
	vec2() :x(0), y(0) {}
	vec2(GLFix x, GLFix y) : x(x), y(y) {}
	vec2(const vec2& v) : x(v.x), y(v.y) {}
    
    //bool operator==(const vec2& v) {
    //    return v.x == x && v.y = y;
    //}
    
	vec2& operator=(const vec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}
	
	vec2 operator+(vec2& v) {
		return vec2(x + v.x, y + v.y);
	}
	vec2 operator-(vec2& v) {
		return vec2(x - v.x, y - v.y);
	}
	
	vec2& operator+=(vec2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}
	vec2& operator-=(vec2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	
	//vec2 operator+(float s) {
	//	return vec2(x + s, y + s);
	//}
	//vec2 operator-(float s) {
	//	return vec2(x - s, y - s);
	//}

	//vec2 operator*(float s) {
	//	return vec2(x * s, y * s);
	//}
	//vec2 operator/(float s) {
	//	return vec2(x / s, y / s);
	//}
    //
	//vec2 operator*(int s) {
	//	return vec2(x * s, y * s);
	//}
	//vec2 operator/(int s) {
	//	return vec2(x / s, y / s);
	//}
	
	vec2 operator*(GLFix s) {
		return vec2(x * s, y * s);
	}
	vec2 operator/(GLFix s) {
		return vec2(x / s, y / s);
	}
	
	//vec2& operator+=(float s) {
	//	x += s;
	//	y += s;
	//	return *this;
	//}
	//vec2& operator-=(float s) {
	//	x -= s;
	//	y -= s;
	//	return *this;
	//}
	//vec2& operator*=(float s) {
	//	x *= s;
	//	y *= s;
	//	return *this;
	//}
	//vec2& operator/=(float s) {
	//	x /= s;
	//	y /= s;
	//	return *this;
	//}
	//
	//vec2& operator*=(int s) {
	//	x *= s;
	//	y *= s;
	//	return *this;
	//}
	//vec2& operator/=(int s) {
	//	x /= s;
	//	y /= s;
	//	return *this;
	//}

	vec2& operator*=(GLFix s) {
		x *= s;
		y *= s;
		return *this;
	}
	vec2& operator/=(GLFix s) {
		x /= s;
		y /= s;
		return *this;
	}
	
	//void rotate(float deg) {
	//	float theta = deg / 180.0 * M_PI;
	//	float c = cos(theta);
	//	float s = sin(theta);
	//	float tx = x * c - y * s;
	//	float ty = x * s + y * c;
	//	x = tx;
	//	y = ty;
	//}
	//
	//void normalize() {
	//	if (length() != 0)
    //        *this *= (1.0 / length());
	//	//return *this;
	//}
	//
	//vec2& normalized() {
	//	if (length() == 0) return vec2(this);
    //    vec2 o = this;
	//	o *= (1.0 / length());
	//	return o;
	//}
	
	//float dist(vec2 v) const {
	//	vec2 d(v.x - x, v.y - y);
	//	return d.length();
	//}
    //
	//float length() const {
	//	return std::sqrt(x * x + y * y);
	//}
	
	vec2 ortho() const {
		return vec2(y, -x);
	}
	
	static float dot(vec2 v1, vec2 v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}
	static float cross(vec2 v1, vec2 v2) {
		return (v1.x * v2.y) - (v1.y * v2.x);
	}
	
};

#endif