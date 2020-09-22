#include <string>
#include <fstream>

inline static void block_decryption_with_key(std::string& block16, const std::string& key_16bytes) noexcept {
	*(unsigned long long*)&block16[0] += *(unsigned long long*)&key_16bytes[0];														// In memorie (la nivel de biti), adunarea si scaderea nu tin cont daca variabila de tip intreg este "signed" sau "unsigned".
	*((unsigned long long*)&block16[0] + 1) += *(const_cast<unsigned long long*>(reinterpret_cast<const unsigned long long*>(&key_16bytes[0])) + 1);	// works, This C++ cast syntax <=> with C-style cast syntax.
}

void reverse_matrix_permutation(std::string& block16) noexcept;
void block16_to_block13(std::string& block16) noexcept;
void reverse_bit_permutation(std::string& block13) noexcept;
void block13_to_block10(std::string& block13) noexcept;
void right_shift_in_memory(std::string& block10) noexcept;
void block10_to_block8(std::string& block10) noexcept;

inline static void block_decryption_without_key(std::string& block16) noexcept {
	reverse_matrix_permutation(block16);										// Block-ul de 16 bytes este vazut ca o matrice 4x4. Linia 1 se va memora pe coloana 2, linia 2
																						// se va memora pe coloana 1, linia 3 se va memora pe coloana 4, linia 4 se va memora pe coloana 3.
	block16_to_block13(block16);													// Eliminam bytes de pe indecsii '0', '9' si '13' ai block-ului de 16 bytes.
	reverse_bit_permutation(block16);											// Permutare de biti in interiorul block-ului ce contine 13 bytes.
	block13_to_block10(block16);													// Eliminam bytes de pe indecsii '2', '5' si '9' ai block-ului de 13 bytes.
	right_shift_in_memory(block16);												// Toti bitii care compun block-ul de 10 bytes se vor permuta la dreapta (de la index mic la index mare) cu 2 biti; fara a se pierde vreun bit.
	block10_to_block8(block16);													// Eliminam bytes de pe indecsii '1' si '6' ai block-ului de 10 bytes.
	*(unsigned long long*) &block16[0] = ~*reinterpret_cast<unsigned long long*> (&block16[0]);		// Toti bitii care compun block-ul de 8 bytes isi vor modifica valoarea.
	// '*' si '~' au aceiasi prioritate, insa ordinea operatiilor este: de la dreapta la stanga.
	{
		unsigned char first_after_read = block16[7];							// Permutare la stanga asupra valorii cu 5 biti.
		first_after_read >>= 3;
		*(unsigned long long*) &block16[0] <<= 5;
		block16[0] |= first_after_read;
	}
}

bool file_decrypt(const std::string& input_file_name, const std::string& output_file_name, const std::string& key_16bytes, std::string initialization_vector) noexcept {
	std::ifstream fin(input_file_name, std::ios::binary);
	fin.seekg(0, fin.end);
	const long long& length = fin.tellg();
	std::ofstream fout;
	if (length > 0) {										// Daca fisierul exista si contine cel putin 1 caracter.
		fin.seekg(0, fin.beg);
		std::string block16;
		std::string next_iv;
		next_iv.resize(8);
		fout.open(output_file_name, std::ios::binary);
		unsigned char block16_index;
		for (unsigned long long counter = 0, no_of_block16 = length / 16; counter < no_of_block16; ++counter) {	// Pentru fiecare block de 16 bytes din fisier.
			block16.resize(16);
			fin.read(&block16[0], 16);																			// Citim block-ul curent in string-ul "block16".
			for (block16_index = 0; block16_index < 16; block16_index += 2) {							// Se vor alege (8) bytes de pe indecsii pari ai ciphertext-ului (de 16 bytes) pentru a forma un string de dimensiune
				next_iv[block16_index / 2] = block16[block16_index];										// egala cu readble block-ul obtinut in urma decriptarii.
			}
			block_decryption_with_key(block16, key_16bytes);
			block_decryption_without_key(block16);
			*(unsigned long long*)&block16[0] ^= *(unsigned long long*)&initialization_vector[0];		// PCBC (Propagating cipher block chaining).
			fout << block16;																						// throw decrypted block to the file.
			initialization_vector = next_iv;
			*(unsigned long long*)&initialization_vector[0] ^= *(unsigned long long*)&block16[0];		// Se efectueaza XOR dintre bytes string-ului format anterior si readble block-ul care tocmai a fost obtinut in urma decriptarii, cu scopul
		}																												// de a face XOR dintre rezultatul operatiei anterioare si urmatorul encrypted block: PCBC (Propagating cipher block chaining).
		const unsigned char& length_last_block = length % 16;
		if (length_last_block) {														// Daca "length_last_block" este numar par si apartine intervalului [2 ; 14], do:
			block16.resize(length_last_block);
			fin.read(&block16[0], length_last_block);							// read encrypted_block from file.
			const unsigned char& length_decrypted_last_block = length_last_block / 2;
			for (unsigned char index = 0; index < length_decrypted_last_block; ++index) {
				block16[index * 2] ^= key_16bytes[index * 2];					// block_decryption_with_key().
				{
					block16[index] = block16[index * 2];							// block_decryption_without_key().
					block16[index] -= 60;
				}
				block16[index] ^= initialization_vector[index];					// PCBC (Propagating cipher block chaining).
			}
			block16.resize(length_decrypted_last_block);
			fout << block16;														// throw readble_block to the file.
		}
	}
	else if (length == 0) {															// Daca fisierul exista, dar este gol.
		fout.open(output_file_name, std::ios::binary);							// Se va crea un fisier gol.
	}
	else {																				// Daca fisierul NU exista (length == -1).
		return false;																	// std::cout << "Fisierul (pe care vrei sa il decriptezi) cu numele: " << input_file_name << " NU exista !\n";
	}
	return true;
}