#include <string>
#include <bitset>

static void bits_swap(const unsigned char& LeftByte_LeftBit, const unsigned char& LeftByte_RightBit, char& Left_Byte, const unsigned char& RightByte_LeftBit, const unsigned char& RightByte_RightBit, char& Right_Byte) noexcept;
static void bit_swap(const unsigned char& LeftByte_Bit, char& Left_Byte, const unsigned char& RightByte_Bit, char& Right_Byte) noexcept;

void bit_permutation(std::string& block13) noexcept {		// Encryption.
	bits_swap(1, 4, block13[1], 1, 4, block13[7]);
	bits_swap(5, 8, block13[1], 4, 7, block13[4]);			// Permuta bitii din intervalul [5 ; 8] ai byte-ului/caracterului "block13[1]" cu bitii din intervalul [4 ; 7] ai byte-ului/caracterului "block13[4]" .
	bits_swap(1, 4, block13[8], 5, 8, block13[0]);
	bits_swap(5, 8, block13[8], 3, 6, block13[11]);
	bits_swap(2, 4, block13[2], 3, 5, block13[8]);
	bits_swap(6, 8, block13[2], 5, 7, block13[11]);
	bit_swap(1, block13[2], 3, block13[4]);				// Permuta primul bit al byte-ului/caracterului "block13[2]" cu bitul 3 al byte-ului/caracterului "block13[4]" .
	bit_swap(5, block13[2], 4, block13[4]);
	bit_swap(3, block13[5], 5, block13[4]);
	bits_swap(6, 8, block13[5], 4, 6, block13[10]);
	bits_swap(1, 2, block13[5], 6, 7, block13[12]);
	bit_swap(4, block13[5], 8, block13[12]);
	bit_swap(5, block13[5], 1, block13[3]);				// <=> bits_swap(5, 5, block13[5], 1, 1, block13[3]);
	bit_swap(2, block13[9], 2, block13[3]);
	bit_swap(1, block13[9], 3, block13[3]);
	bits_swap(3, 5, block13[9], 5, 7, block13[1]);
	bits_swap(6, 8, block13[9], 2, 4, block13[6]);
}

void reverse_bit_permutation(std::string& block13) noexcept {	// Decryption.
	bits_swap(6, 8, block13[9], 2, 4, block13[6]);				// <=> bits_swap(2, 4, block13[6], 6, 8, block13[9]);
	bits_swap(3, 5, block13[9], 5, 7, block13[1]);
	bit_swap(1, block13[9], 3, block13[3]);
	bit_swap(2, block13[9], 2, block13[3]);
	bit_swap(5, block13[5], 1, block13[3]);					// <=> bit_swap(1, block13[3], 5, block13[5]);
	bit_swap(4, block13[5], 8, block13[12]);
	bits_swap(1, 2, block13[5], 6, 7, block13[12]);
	bits_swap(6, 8, block13[5], 4, 6, block13[10]);
	bit_swap(3, block13[5], 5, block13[4]);
	bit_swap(5, block13[2], 4, block13[4]);
	bit_swap(1, block13[2], 3, block13[4]);
	bits_swap(6, 8, block13[2], 5, 7, block13[11]);
	bits_swap(2, 4, block13[2], 3, 5, block13[8]);
	bits_swap(5, 8, block13[8], 3, 6, block13[11]);
	bits_swap(1, 4, block13[8], 5, 8, block13[0]);
	bits_swap(5, 8, block13[1], 4, 7, block13[4]);
	bits_swap(1, 4, block13[1], 1, 4, block13[7]);
}

static void bits_swap(const unsigned char& LeftByte_LeftBit, const unsigned char& LeftByte_RightBit, char& Left_Byte, const unsigned char& RightByte_LeftBit, const unsigned char& RightByte_RightBit, char& Right_Byte) noexcept {
	/*
	Pentru ca aceasta functie sa poata interschimba intervale de biti, intervalele trebuie sa respecte urmatoarele proprietati:
		1. Cele 2 intervale trebuie sa aibe lungime egala.
		2. Cele 2 intervale trebuie sa fie crescatoare (bitul din dreapta intervalului trebuie sa fie mai mare sau egal decat bitul din stanga intervalului).
	*/
	std::bitset<8> Bitset_Left_Byte = Left_Byte;					// Atunci cand copiem o variabila intr-un obiect de tip 'bitset', bitii variabilei vor ajunge in obiectul de tip 'bitset' in ordinea inversa.
	std::bitset<8> Bitset_Right_Byte = Right_Byte;				// Prin urmare, daca obiectul de tip 'bitset' contine 8 biti si variabila contine 8 biti.
	char Bitset_LeftByte_LeftBit = 8 - LeftByte_LeftBit;			// Atunci al n-lea bit din variabila va deveni al (8 - n)-lea bit din obiectul de tip 'bitset'.
	char Bitset_RightByte_LeftBit = 8 - RightByte_LeftBit;
	const char& LeftRange_minus_RightRange = Bitset_LeftByte_LeftBit - Bitset_RightByte_LeftBit;
	const char& Bitset_LeftByte_RightBit = 7 - LeftByte_RightBit;
	for (; Bitset_LeftByte_LeftBit > Bitset_LeftByte_RightBit; --Bitset_LeftByte_LeftBit) {
		Bitset_Left_Byte[Bitset_LeftByte_LeftBit] = Bitset_Right_Byte[Bitset_LeftByte_LeftBit - LeftRange_minus_RightRange];
	}
	const std::bitset<8> Bitset_Copy_LeftByte = Left_Byte;		// Pastram (in obiect de tip 'bitset') o copie a byte-ului din stanga trimis ca parametru (NEmodificat).
	Left_Byte = *(char*)&Bitset_Left_Byte;							// Atunci cand copiem un obiect de tip 'bitset' intr-o variabila, bitii obiectului de tip 'bitset' vor ajunge in variabila in ordinea inversa.
																	// Prin urmare, daca obiectul de tip 'bitset' contine 8 biti si variabila contine 8 biti.
																	// Atunci al (8 - n)-lea bit din obiectul de tip 'bitset' va deveni al n-lea bit din variabila.
	const char& Bitset_RightByte_RightBit = 7 - RightByte_RightBit;
	for (; Bitset_RightByte_LeftBit > Bitset_RightByte_RightBit; --Bitset_RightByte_LeftBit) {
		Bitset_Right_Byte[Bitset_RightByte_LeftBit] = Bitset_Copy_LeftByte[Bitset_RightByte_LeftBit + LeftRange_minus_RightRange];
	}
	Right_Byte = *(char*)&Bitset_Right_Byte;
}

static void bit_swap(const unsigned char& LeftByte_Bit, char& Left_Byte, const unsigned char& RightByte_Bit, char& Right_Byte) noexcept {
	std::bitset<8> Bitset_Left_Byte = Left_Byte;
	std::bitset<8> Bitset_Right_Byte = Right_Byte;
	const unsigned char& Bitset_LeftByte_bit = 8 - LeftByte_Bit;
	const unsigned char& Bitset_RightByte_bit = 8 - RightByte_Bit;
	Bitset_Left_Byte[Bitset_LeftByte_bit] = Bitset_Right_Byte[Bitset_RightByte_bit];
	const std::bitset<8> Bitset_Copy_LeftByte = Left_Byte;
	Left_Byte = *(char*)&Bitset_Left_Byte;
	Bitset_Right_Byte[Bitset_RightByte_bit] = Bitset_Copy_LeftByte[Bitset_LeftByte_bit];
	Right_Byte = *(char*)&Bitset_Right_Byte;
}