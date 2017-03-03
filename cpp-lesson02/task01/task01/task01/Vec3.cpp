#include "Vec3.h"
#include <math.h>
#include <iostream>
#include <string>
#include <sstream>

Vec3::Vec3() {

}

Vec3::~Vec3() {

}

Vec3::Vec3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vec3::length() {
	return sqrt(pow(x, 2) * pow(y, 2) * pow(z, 2));
}

void Vec3::add(Vec3 v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
}

Vec3 Vec3::addVectors(Vec3 a, Vec3 b) {
	return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

std::string Vec3::toString() {
	std::string str1;
	std::string str2;
	std::string str3;
	std::stringstream s;
	s << x;
	s << ",  ";
	s >> str1;
	s << y;
	s << ",  ";
	s >> str2;
	s << z;
	s >> str3;
	return str1 + str2 + str3;
}

std::ostream &operator<<(std::ostream& os, Vec3& v) {
		os << "len: " << v.length;
		return os;
}

