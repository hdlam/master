#ifndef Vector2D_guard

#include <Arduino.h>
#include <math.h>
#define Vector2D_guard

class Vector2D
{
	public:
		Vector2D(float x = 0, float y = 0);
		float length() const;
		float lengthSquared() const;
		float normalize();
		float cross(const Vector2D& v2) const;
		float dot(const Vector2D& v2) const;
        void add(const Vector2D& v2);
        void sub(const Vector2D& v2);
        void div(float d); 
        void limit(float limit);
        void mul(float m);
        void reset();
        float dist(const Vector2D& v2) const;
        void rotate(float rotAmount);
	public:
		float x, y;

};
#endif
