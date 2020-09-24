#include <iostream>
#include <conio.h>
#include <string>
//#include <chrono>
#include "encryption_decryption.h"
#include <fstream>

int main( /*int argument_counter, char* argument_vector[]*/ ) {
	std::ifstream fin("encryption_key.txt", std::ios::binary);
	std::string key;
	getline(fin, key);
	// std::cout << key << "\n";
	std::string input_file_name, output_file_name;

	std::cout << "Press 'E' for Encryption or 'D' for Decryption: ";
	char option;
	std::cin >> option;
	if (option == 'E') {
		std::cout << "Enter the name of the file that you want to encrypt:\n\t";
		std::cin >> input_file_name;
		std::cout << "Enter the name of the encrypted file:\n\t";
		std::cin >> output_file_name;
		//	auto start_point = std::chrono::high_resolution_clock::now();
		file_encrypt(input_file_name, output_file_name, key, "s0tIu*cj");
		//	auto end_point = std::chrono::high_resolution_clock::now();
		//	auto lenght_of_interval = std::chrono::duration_cast<std::chrono::milliseconds>(end_point - start_point);
		//	std::cout << "Time taken by function: " << lenght_of_interval.count() << " milliseconds\n";
	}
	else if (option == 'D') {
		std::cout << "Enter the name of the file that you want to decrypt:\n\t";
		std::cin >> input_file_name;
		std::cout << "Enter the name of the decrypted file:\n\t";
		std::cin >> output_file_name;
		//	auto start_point = std::chrono::high_resolution_clock::now();
		file_decrypt(input_file_name, output_file_name, key, "s0tIu*cj");
		//	auto end_point = std::chrono::high_resolution_clock::now();
		//	auto lenght_of_interval = std::chrono::duration_cast<std::chrono::milliseconds>(end_point - start_point);
		//	std::cout << "Time taken by function: " << lenght_of_interval.count() << " milliseconds\n";
	}
	else {
		std::cout << "Pressed keyword doesn't match any option\n";
	}
	std::cout << "\nPress any key to continue...";
	_getch();
}