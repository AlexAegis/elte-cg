#include<iostream>
#define HELLO "Hello World!"

int main() {
	std::cout << HELLO << std::endl;
	auto i = 10;
	std::cout << i << std::endl;
	i = 20;
	system("pause"); //std::cin.get();
	return 0;
}