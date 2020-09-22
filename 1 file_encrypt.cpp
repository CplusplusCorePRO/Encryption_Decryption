#include <string>
#include <fstream>

void block8_to_block10(std::string& block8) noexcept;
void left_shift_in_memory(std::string& block10) noexcept;
void block10_to_block13(std::string& block10) noexcept;
void bit_permutation(std::string& block13) noexcept;
void block13_to_block16(std::string& block13) noexcept;
void matrix_permutation(std::string& block16) noexcept;

inline static void block_encryption_without_key(std::string& block8) noexcept {
	{
		unsigned char last_after_read = block8[0];										// Permutare la dreapta asupra valorii cu 5 biti.
		last_after_read <<= 3;
		*(unsigned long long*) &block8[0] >>= 5;
		block8[7] |= last_after_read;
	}
	*(unsigned long long*) &block8[0] = ~*(unsigned long long*) &block8[0];	// Toti bitii care compun block-ul de 8 bytes isi vor modifica valoarea.
	// '*' si '~' au aceiasi prioritate, insa ordinea operatiilor este: de la dreapta la stanga.
	block8_to_block10(block8);															// Se creaza si adauga 2 bytes pe indecsii '1' si '6' ai block-ului ce va contine 10 bytes.
	left_shift_in_memory(block8);														// Toti bitii care compun block-ul de 10 bytes se vor permuta la stanga (de la index mare la index mic) cu 2 biti; fara a se pierde vreun bit.
	block10_to_block13(block8);															// Se creeaza si adauga 3 bytes pe indecsii '2', '5' si '9' ai block-ului ce va contine 13 bytes.
	bit_permutation(block8);																// Permutare de biti in interiorul block-ului ce contine 13 bytes.
	block13_to_block16(block8);															// Se creeaza si adauga 3 bytes pe indecsii '0', '9' si '13' ai block-ului ce va contine 16 bytes.
	matrix_permutation(block8);															// Block-ul de 16 bytes este vazut ca o matrice 4x4. Coloana 2 se va memora pe linia 1, coloana 1
}																								// se va memora pe linia 2, coloana 4 se va memora pe linia 3, coloana 3 se va memora pe linia 4.

inline static void block_encryption_with_key(std::string& block16, const std::string& key_16bytes) noexcept {
	*(long long*)&block16[0] -= *reinterpret_cast<long long*>(const_cast<char*>(&key_16bytes[0]));		// In memorie (la nivel de biti), adunarea si scaderea nu tin cont daca variabila de tip intreg este "signed" sau "unsigned".
	*((long long*)&block16[0] + 1) -= *((long long*)&key_16bytes[0] + 1);
}

bool file_encrypt(const std::string& input_file_name, const std::string& output_file_name, const std::string& key_16bytes, std::string initialization_vector) noexcept {
	std::ifstream fin(input_file_name, std::ios::binary);
	fin.seekg(0, fin.end);
	const long long& length = fin.tellg();
	std::ofstream fout;
	if (length > 0) {								// Daca fisierul exista si contine cel putin 1 caracter.
		fin.seekg(0, fin.beg);
		std::string block8;
		std::string current_readble_block;
		current_readble_block.resize(8);
		fout.open(output_file_name, std::ios::binary);
		unsigned char block8_index;
		for (unsigned long long counter = 0, no_of_block8 = length / 8; counter < no_of_block8; ++counter) {	// Pentru fiecare block de 8 bytes din fisier.
			block8.resize(8);
			fin.read(&block8[0], 8);																			// Citim block-ul curent in string-ul "block8".
			current_readble_block = block8;
			*(unsigned long long*)&block8[0] ^= *(unsigned long long*)&initialization_vector[0];	// PCBC (Propagating cipher block chaining).
			block_encryption_without_key(block8);
			block_encryption_with_key(block8, key_16bytes);
			fout << block8;																						// throw encrypted block to the file.
			for (block8_index = 0; block8_index < 16; block8_index += 2) {								// Se vor alege (8) bytes de pe indecsii pari ai ciphertext-ului (de 16 bytes) pentru a forma un string de dimensiune
				initialization_vector[block8_index / 2] = block8[block8_index];							// egala cu urmatorul readble block.
			}
			*(unsigned long long*)&initialization_vector[0] ^= *(unsigned long long*)&current_readble_block[0];	// Se efectueaza XOR dintre bytes string-ului format anterior si readble block-ul care tocmai a fost criptat, cu scopul
		}																											// de a face XOR dintre rezultatul operatiei anterioare si urmatorul readble block: PCBC (Propagating cipher block chaining).
		const unsigned char& length_last_block = length % 8;
		if (length_last_block) {																					// Daca "length_last_block" apartine intervalului [1 ; 7], do:
			const unsigned char& length_encrypted_last_block = 2 * length_last_block;
			block8.resize(length_encrypted_last_block);
			fin.read(&block8[0], length_last_block);															// read readble_block from file.
			unsigned char index;
			for (index = 0; index < length_last_block; ++index) {											// PCBC (Propagating cipher block chaining).
				block8[index] ^= initialization_vector[index];
			}
			for (index = length_last_block - 1; index > 0; --index) {
				block8[index * 2] = block8[index];															// block_encryption_without_key().
			}
			for (index = 1; index < length_encrypted_last_block; index += 2) {
				block8[index] = block8[index - 1] ^ 85;
			}
			for (index = 0; index < length_encrypted_last_block; index += 2) {
				block8[index] += 60;
			}
			for (index = 0; index < length_encrypted_last_block; ++index) {							// block_encryption_with_key().
				block8[index] ^= key_16bytes[index];
			}
			fout << block8;																						// throw encrypted_block to the file.
		}
	}
	else if (length == 0) {													// Daca fisierul exista, dar este gol.
		fout.open(output_file_name, std::ios::binary);					// Se va crea un fisier gol.
	}
	else {																		// Daca fisierul NU exista (length == -1).
		return false;															// std::cout << "Fisierul (pe care vrei sa il criptezi) cu numele: " << input_file_name << " NU exista !\n";
	}
	return true;
}