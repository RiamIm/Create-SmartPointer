#include <iostream>
#include <string>
#include <memory>

#include "SmartPointer.h"

struct Dog {
	std::string name;
	int age;
};

int main() {

	{
		std::cout << "===== unique_ptr =====\n";

		hwang::unique_ptr<Dog> dogPtr(new Dog());
		dogPtr->age = 10;
		dogPtr->name = "개";

		std::cout << dogPtr->age << '\n';
		std::cout << dogPtr->name << '\n';

		hwang::unique_ptr<int> ptr(new int(10));
		int* raw = ptr.release();
		std::cout << *raw << '\n';
		delete raw; // 소유권 이전 -> 직접 delete

		hwang::unique_ptr<int> ptr2(new int(10));
		ptr2.reset(new int(20));
		std::cout << *ptr2 << '\n';
		ptr2.reset();
		std::cout << ptr2.get() << '\n';

		std::cout << "===== make_unique 함수 사용 ===== \n";
		auto a = hwang::make_unique<int>(10);
		auto b = hwang::make_unique<Dog>("Buddy", 10);

		std::cout << *a << '\n';
		std::cout << b->name << '\n';

	}
	
	{
		std::cout << "\n===== shared_ptr =====\n";
		hwang::shared_ptr<int> a(new int(10));
		hwang::shared_ptr<int> b = a;

		std::cout << *a << '\n';
		std::cout << a.get() << '\n';
		std::cout << b.get() << '\n';
		std::cout << a.use_count() << '\n';

		hwang::shared_ptr<int> c(new int(10));
		hwang::shared_ptr<int> d(new int(20));

		std::cout << c.use_count() << '\n';
		std::cout << d.use_count() << '\n';
	}
	return 0;
}