#include "String.h"


int main()
{
	const char* s = "123324";
	String s1{ s };
	std::cout << "S1 - OUTPUT!" << std::endl;
	std::cout << s1;
	String s2;
	std::cin >> s2;
	std::cout << std::endl;
	std::cout << "S2 - OUTPUT!" << std::endl;
	std::cout << s2 << std::endl;

	String s3 = s1 + s2;
	std::cout << std::endl;
	std::cout << "S3-OUTPUT" << std::endl;
	std::cout << s3;

	std::cout << std::endl << "S4 - OUTPUT: " << std::endl;
	String s4{ "scasasd_4" };
	std::cout << "s4: " << s4;
	s3.insert(3, s4);
	std::cout << s3;


	std::cout << "************** SubStr ***************" << std::endl;
	String s5;
	s5 = s3.substr(3, 7);
	std::cout << "S5-OUTPUT" << std::endl;
	std::cout << s5;
	std::cout << "S3-OUTPUT" << std::endl;
	std::cout << s3;

	std::cout << "************** At[] ***************" << std::endl;
	std::cout << "Pos 3: " << s5.at(3) << std::endl;
	

	std::cout << "************** Push-Pop ***************" << std::endl;
	s5.push_back('a');
	std::cout << s5;
	s5.pop_back();
	std::cout << s5;

	std::cout << std::endl << "Erasing!" << std::endl;
	s3.erase(3, 5);
	std::cout << s3;

	return 0;
}
