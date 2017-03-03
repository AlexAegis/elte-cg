#include <iostream>
#include "Vec3.h"
#include <string>

int main() {
	std::cout << "HW" << std::endl;

	// Calling a constructor
	Vec3 v1 = Vec3(1, 2, 3); 
	Vec3 v2(4, 5, 6);
	std::cout << "v1 length: " << v1.length() << std::endl;
	std::cout << "v2 length: " << v2.length() << std::endl;

	Vec3 v3 = Vec3::addVectors(v1, v2);

	v1.add(v2);

	std::cout << "v1 + v2 length: " << v1.length() << std::endl;
	
	std::cout << v3.toString();

	//std::cout << v3;

	std::getchar();
	return 0;
}