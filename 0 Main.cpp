#include <iostream>
#include <conio.h>
#include <string>
//#include <chrono>
#include "encryption_decryption.h"
#include <fstream>

int main(  ) {
	std::ifstream fin("encryption_key.txt", std::ios::binary);
	std::string key;
	getline(fin, key);

	std::string input_file_name, output_file_name;

	std::cout << "Introduceti numele fisierului pe care vreti sa il criptati:\n\t";
	std::cin >> input_file_name;
	std::cout << "Introduceti numele fisierului care va contine text-ul criptat:\n\t";
	std::cin >> output_file_name;
//	auto start_point = std::chrono::high_resolution_clock::now();
	file_encrypt(input_file_name, output_file_name, key, "s0tIu*cj");
//	auto end_point = std::chrono::high_resolution_clock::now();
//	auto lenght_of_interval = std::chrono::duration_cast<std::chrono::milliseconds>(end_point - start_point);
//	std::cout << "Time taken by function: " << lenght_of_interval.count() << " milliseconds\n";

	std::cout << "Introduceti numele fisierului pe care vreti sa il decriptati:\n\t";
	std::cin >> input_file_name;
	std::cout << "Introduceti numele fisierului care va contine text-ul decriptat:\n\t";
	std::cin >> output_file_name;
//	start_point = std::chrono::high_resolution_clock::now();
	file_decrypt(input_file_name, output_file_name, key, "s0tIu*cj");
//	end_point = std::chrono::high_resolution_clock::now();
//	lenght_of_interval = std::chrono::duration_cast<std::chrono::milliseconds>(end_point - start_point);
//	std::cout << "Time taken by function: " << lenght_of_interval.count() << " milliseconds\n";
	
	std::cout << "\nPress any key to continue...";
	_getch();
}