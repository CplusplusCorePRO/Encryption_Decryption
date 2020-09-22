#include <string>
#include <bitset>

void left_shift_in_memory(std::string& block10) noexcept {	// Encryption.
	std::bitset<80> bitset_block10;
	// unsigned char* const UChar_to_Bitset = (unsigned char*) &bitset_block10;
	unsigned char* const UChar_to_Bitset = reinterpret_cast<unsigned char*>(&bitset_block10);	// &bitset_block10 == UChar_to_Bitset[0] == adresa de memorie a bitilor de pe indecsii [0 , 7] ai obiectului de tip 'bitset'.
	unsigned char index;
	for (index = 0; index < 10; ++index) {
		UChar_to_Bitset[9 - index] = block10[index];		// Copiem string-ul 'block10' in obiectul de tip 'bitset'.
	}																// UChar_to_Bitset[1] == adresa de memorie a bitilor de pe indecsii [8 , 15] ai obiectului de tip 'bitset'.
																	// Bytes: 0, 1, ..., 9 ai lui block10 se vor memora pe bytes 9, 8, ... , 0 ai lui 'bitset_block10'.
	std::bitset<2> aux_bitset;									// Cand memoram bitii unui obiect (de orice tip) in 'bitset', 'bitset'-ul va stoca bitii in ordine inversa, si anume:
	aux_bitset[0] = bitset_block10[78];						// primul bit al obiectului (de orice tip) se va afla in ultimul index al 'bitset'-ului; al 2-lea bit al obiectului (de orice tip)
	aux_bitset[1] = bitset_block10[79];						// se va afla in penultimul index al 'bitset'-ului etc.
	bitset_block10 <<= 2;										// Left-shift pe un obiect de tip 'bitset' va permuta bitii de la index mic catre index mare.
	bitset_block10[0] = aux_bitset[0];
	bitset_block10[1] = aux_bitset[1];
	for (index = 0; index < 10; ++index) {
		block10[index] = UChar_to_Bitset[9 - index];		// Copiem obiectul de tip 'bitset' inapoi in 'block10';
	}
}

void right_shift_in_memory(std::string& block10) noexcept {	// Decryption.
	std::bitset<80> bitset_block10;
	unsigned char* const UChar_to_Bitset = (unsigned char*) &bitset_block10;
	unsigned char index;
	for (index = 0; index < 10; ++index) {
		UChar_to_Bitset[9 - index] = block10[index];
	}
	std::bitset<2> aux_bitset;
	aux_bitset[0] = bitset_block10[0];
	aux_bitset[1] = bitset_block10[1];
	bitset_block10 >>= 2;									// Right-shift pe un obiect de tip 'bitset' va permuta bitii de la index mare catre index mic.
	bitset_block10[78] = aux_bitset[0];
	bitset_block10[79] = aux_bitset[1];
	for (index = 0; index < 10; ++index) {
		block10[index] = UChar_to_Bitset[9 - index];
	}
}