#include "simulation.h"
#include <string>
#include <sstream>
using std::cout;
using std::cin;

simOptions getCFG()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	string input;

	cout << "Program Options\n\n";
	simOptions s;
	cout << "Console Output (true or false): ";
	std::getline(std::cin, input);
	std::stringstream(input) >> s.console_print;
	
	cout << "Start Size (2-20): ";
	std::getline(std::cin, input);
	std::stringstream(input) >> s.start_size;
	
	cout << "Max Size (100-1048576): ";
	std::getline(std::cin, input);
	std::stringstream(input) >> s.max_size;
	
	cout << "Mutation Rate (0.00000-1.00000; or rnd): ";
	std::getline(std::cin, input);
	
	if (input == "rnd") {
		std::uniform_int_distribution<> dis(000000, 100000);
		s.mut_rate = dis(gen) / static_cast<float>(100000);
	}
	else s.mut_rate = (float)atof(input.c_str());

	cout << "Max Life (5-120): ";
	std::getline(std::cin, input);
	std::stringstream(input) >> s.max_life;
	
	cout << "Average Life (3-120): ";
	std::getline(std::cin, input);
	std::stringstream(input) >> s.avg_life;
	
	cout << "Generation Length (1-40): ";
	std::getline(std::cin, input);
	std::stringstream(input) >> s.gen_length;
	
	cout << "Max Children (1-80): ";
	std::getline(std::cin, input);
	std::stringstream(input) >> s.max_childs;
	
	cout << "Max Generations (20-209714): ";
	std::getline(std::cin, input);
	std::stringstream(input) >> s.console_print;
	
	cout << "Natural Selection (0.00-1.00): ";
	std::getline(std::cin, input);
	
	if (input == "rnd") {
		std::uniform_int_distribution<> dis(000, 100);
		s.nat_sel = dis(gen) / static_cast<float>(100);
	}
	else s.nat_sel = (float)atof(input.c_str());

	cout << "Environmental Change Rate (0.000-1.000): ";
	std::getline(std::cin, input);
	
	if (input == "rnd") {
		std::uniform_int_distribution<> dis(0000, 1000);
		s.env_change = dis(gen) / static_cast<float>(1000);
	}
	else s.env_change = (float)atof(input.c_str());

	cout << "Environmental Dominance (0.00-1.00): ";
	std::getline(std::cin, input);
	
	if (input == "rnd") {
		std::uniform_int_distribution<> dis(000, 100);
		s.env_dominance = dis(gen) / static_cast<float>(100);
	}
	else s.env_dominance = (float)atof(input.c_str());

	cout << "Initial Food Count (2-2000): ";
	std::getline(std::cin, input);

	if (input == "rnd") {
		std::uniform_int_distribution<> dis(2, 2000);
		s.initial_food = dis(gen);
	}
	else s.initial_food = (uint16_t)atoi(input.c_str());
	

	return s;
}
