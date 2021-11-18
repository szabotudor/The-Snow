#include<Vector.h>
#include<math.h>

/*
=======================
Vector Code
=======================
*/

ss::Vector::Vector(float x, float y) {
	Vector::x = x;
	Vector::y = y;
}

ss::Vector::Vector(float w) {
	Vector::x = w;
	Vector::y = w;
}

//Classic number operators
ss::Vector ss::Vector::operator+(float x) {
	return Vector(this->x + x, this->y + x);
}

ss::Vector ss::Vector::operator-(float x) {
	return Vector(this->x - x, this->y - x);
}

ss::Vector ss::Vector::operator*(float x) {
	return Vector(this->x * x, this->y * x);
}

ss::Vector ss::Vector::operator/(float x) {
	return Vector(this->x / x, this->y / x);
}

//Auto operators
ss::Vector ss::Vector::operator+=(float x) {
	return Vector(this->x + x, this->y + x);
}

ss::Vector ss::Vector::operator-=(float x) {
	return Vector(this->x - x, this->y - x);
}

ss::Vector ss::Vector::operator*=(float x) {
	return Vector(this->x * x, this->y * x);
}

ss::Vector ss::Vector::operator/=(float x) {
	return Vector(this->x / x, this->y / x);
}

//Full vector operators
ss::Vector ss::Vector::operator+(Vector v) {
	return Vector(this->x + v.x, this->y + v.y);
}

ss::Vector ss::Vector::operator-(Vector v) {
	return Vector(this->x - v.x, this->y - v.y);
}

ss::Vector ss::Vector::operator*(Vector v) {
	return Vector(this->x * v.x, this->y * v.y);
}

ss::Vector ss::Vector::operator/(Vector v) {
	return Vector(this->x / v.x, this->y / v.y);
}

//Set operator
ss::Vector ss::Vector::operator=(float x) {
	return Vector(x, x);
}

ss::Vector ss::Vector::normalized() {
	float n = this->lenght();
	return Vector(this->x / n, this->y / n);
}

void ss::Vector::normalize() {
	float n = this->lenght();
	x = this->x / n;
	y = this->y / n;
}

float ss::Vector::lenght() {
	return (float)sqrt((double)this->x * this->x + (double)this->y * this->y);
}


/*
=======================
iVector Code
=======================
*/

ss::iVector::iVector(int x, int y) {
	iVector::x = x;
	iVector::y = y;
}

ss::iVector::iVector(int w) {
	iVector::x = w;
	iVector::y = w;
}

//Classic number operators
ss::iVector ss::iVector::operator+(int x) {
	return iVector(this->x + x, this->y + x);
}

ss::iVector ss::iVector::operator-(int x) {
	return iVector(this->x - x, this->y - x);
}

ss::iVector ss::iVector::operator*(int x) {
	return iVector(this->x * x, this->y * x);
}

ss::iVector ss::iVector::operator/(int x) {
	return iVector(this->x / x, this->y / x);
}

//Auto operators
ss::iVector ss::iVector::operator+=(int x) {
	return iVector(this->x + x, this->y + x);
}

ss::iVector ss::iVector::operator-=(int x) {
	return iVector(this->x - x, this->y - x);
}

ss::iVector ss::iVector::operator*=(int x) {
	return iVector(this->x * x, this->y * x);
}

ss::iVector ss::iVector::operator/=(int x) {
	return iVector(this->x / x, this->y / x);
}

//Full iVector operators
ss::iVector ss::iVector::operator+(iVector v) {
	return iVector(this->x + v.x, this->y + v.y);
}

ss::iVector ss::iVector::operator-(iVector v) {
	return iVector(this->x - v.x, this->y - v.y);
}

ss::iVector ss::iVector::operator*(iVector v) {
	return iVector(this->x * v.x, this->y * v.y);
}

ss::iVector ss::iVector::operator/(iVector v) {
	return iVector(this->x / v.x, this->y / v.y);
}

//Set operator
ss::iVector ss::iVector::operator=(int x) {
	return iVector(x, x);
}


float ss::iVector::lenght() {
	return (float)sqrt((double)this->x * this->x + (double)this->y * this->y);
}
