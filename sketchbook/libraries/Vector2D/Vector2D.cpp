#include "Vector2D.h"

/*Vector2D::Vector2D(){
    this->x = 0;
    this->y = 0;
}*/

Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

float Vector2D::cross(const Vector2D& v2) const
{
	return x * v2.y - y * v2.y;
}

float Vector2D::dot(const Vector2D& v2) const
{
	return x * v2.x + y * v2.y;
}

float Vector2D::lengthSquared() const 
{
	return dot(*this);
}

float Vector2D::length() const
{
	return sqrt(lengthSquared());
}

float Vector2D::normalize()
{
	float mag = length();

	if (mag == 0.0) 
		return mag;

	x /= mag;
	y /= mag;

	return mag;
}
void Vector2D::add(const Vector2D& v2){
    x += v2.x;
    y += v2.y;
}
void Vector2D::sub(const Vector2D& v2){
    x = x-v2.x;
    y = y-v2.y;
}

void Vector2D::div(float d){ 
    if(d == 0)
        return;
    x = x/d;
    y = y/d;
}
void Vector2D::mul(float m){ 
    x = x*m;
    y = y*m;
}

void Vector2D::limit(float limit){
    if(x > limit)
        x = limit;
    else if(x < -limit)
        x = -limit;
    if(y > limit)
        y = limit;
    else if(y < -limit)
        y = -limit;
}

void Vector2D::reset(){
    x = 0;
    y = 0;
}

float Vector2D::dist(const Vector2D& v2) const{
    return sqrt(pow(v2.x-x,2)+pow(v2.y-y,2));    
}

void Vector2D::rotate(float rotAmount){
    float px = x*cos(rotAmount) - y*sin(rotAmount);
    float py = x*sin(rotAmount) + y*cos(rotAmount);
    x = px;
    y = py;
}
