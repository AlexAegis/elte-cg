#include <iostream>
#include <vector>

void printAdd(int a, int b) {
	std::cout << a + b << std::endl;
}

int add(int a, int b) {
	return a + b;
}

void incrementNonReferenced(int a) {
	a = a + 1;
}

void incrementReferenced(int &a) {
	a = a + 1;
}

void multiplyEveryElementByTen(std::vector<int> &v) {
	for (int &a : v) {
		a *= 10;
	}
}

void printEveryElement(std::vector<int> v) {
	for (int a : v) {
		std::cout << a << ", ";
	}
	std::cout << std::endl;
}

// fP(&a) == fR(a)

void fP(int* a) {
	(*a) = 7;
}

void fR(int &a) {
	a = 7;
}

int main() {

	// variable
	int a = 5;

	std::cout << a << std::endl;

	std::cout << std::endl;

	// array
	int t[10];
	std::cout << "t[2] uninitailized = " << t[2] << std::endl;
	for(int i = 0; i < 10; i++) {
		t[i] = 0;
	}

	std::cout << std::endl;

	//referencing
	std::cout << "printAdd 2, 10 = ";
	printAdd(2, 10);
	std::cout << "add 2, 10 = " << add(2, 10) << std::endl;

	int one = 1;
	std::cout << "one = " << one << " after incrementNonReferenced(one) = ";
	incrementNonReferenced(one);
	std::cout << one << std::endl;

	std::cout << "one = " << one << " after incrementReferenced(one) = ";
	incrementReferenced(one);
	std::cout << one << std::endl;

	std::cout << std::endl;

	// Vector
	std::vector<int> v;

	for (int i = 0; i < 10; i++)
	{
		v.push_back(rand() % 1000);
	}
	std::cout << "random vector: ";
	printEveryElement(v);
	std::cout << "multiplied by 10 vector: ";
	multiplyEveryElementByTen(v);
	printEveryElement(v);

	std::cout << std::endl;

	// pointerek
	int two = 1;
	std::cout << "two = " << two << " after fP(two) = ";
	fP(&two);
	std::cout << two << std::endl;

	two = 1;
	std::cout << "two = " << two << " after fR(two) = ";
	fR(two);
	std::cout << two << std::endl;

	int* p = &two;

	std::cout << "int* p = &two; -> p = " << p << std::endl;

	p = p + 1;

	std::cout << "p = p + 1; -> p = " << p << std::endl;





	std::cin.get();
	return 0;
}