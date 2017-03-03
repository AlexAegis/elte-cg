#pragma once
#include <string>
class Vec3 {
private:
	float x;
	float y;
	float z;

public:
	Vec3();
	Vec3(float x, float y, float z);
	~Vec3();

	float getX() { return this->x; };
	float getY() { return this->y; };
	float getZ() { return this->z; };

	float length();

	void add(Vec3 v);

	static Vec3 addVectors(Vec3 a, Vec3 b);
	std::string toString();
};