#include <string>
#include <bitset>

static void bits_copy(const char& Left_Byte, const unsigned char& LeftByte_LeftBit, const unsigned char& LeftByte_RightBit, char& Right_Byte, const unsigned char& RightByte_RightBit) noexcept;
static void bit_copy(const char& Left_Byte, const unsigned char& LeftByte_bit, char& Right_Byte, const unsigned char& RightByte_bit) noexcept;

void block13_to_block16(std::string& block13) noexcept {	// Encryption.
	{
		block13.resize(16);
		block13[15] = block13[12];							// Cei 13 bytes deja existenti se vor permuta la dreapta (de la index mic la index mare) in cei 16 indecsi disponibili astfel incat
		block13[14] = block13[11];							// cei 3 bytes noi sa se creeze pe indecsii : 0, 9 si 13.
		unsigned char index;
		for (index = 12; index > 9; --index) {
			block13[index] = block13[index - 2];
		}
		for (index = 8; index > 0; --index) {
			block13[index] = block13[index - 1];
		}
	}
	{	// Create new byte on index '13':
		bit_copy(block13[3], 2, block13[13], 1);			// Copiem bitul 2 al byte-ului block13[3] in bitul 1 al byte-ului block13[3].
		bit_copy(block13[5], 4, block13[13], 2);			// Copiem bitul 4 al byte-ului block13[5] in bitul 2 al byte-ului block13[3].
		bits_copy(block13[12], 4, 5, block13[13], 4);		// Copiem bitii 4 si 5 ai byte-ului block13[12] in bitii 3 si 4 ai byte-ului block13[3].
		bits_copy(block13[15], 2, 5, block13[13], 8);		// Copiem bitii din intervalul [2 ; 5] ai byte-ului block13[15] in bitii din intervalul [5 ; 8] ai byte-ului block13[3].
	}
	{	// Create new byte on index '0':
		std::bitset<8> bits_interval;
		if (block13[3] > block13[6]) {
			bits_interval = block13[12];
		}
		else {
			bits_interval = block13[8];
		}
		bits_interval = bits_interval >> 5 << 5;			// Obiectul 'bits_interval' va memora primii 3 biti ai unuia dintre bytes: block13[12], block13[8]; urmat de biti nuli pana la umplerea celor 8 biti ce formeaza 'bitset'-ul.
		block13[0] = 0;
		block13[0] = *(char*) &bits_interval;				// Atribuim toti cei 8 biti memorati in 'bits_interval', in byte-ul/caracterul "block13[0]" .
		if (block13[2] > block13[7]) {
			bits_interval = block13[4];
		}
		else {
			bits_interval = block13[11];
		}
		bits_interval = bits_interval << 3 >> 5 << 2;	// Obiectul 'bits_interval' va memora bitii din intervalul [4 ; 6] ai unuia dintre bytes: block13[4], block13[11], in bitii din intervalul [4 ; 6] ;
																// restul bitilor ce compun 'bitset'-ul vor avea valoarea '0'.
		block13[0] |= *(char*) &bits_interval;				// Atribuim bitii din intervalul [4 ; 6] ai 'bitset'-ului in bitii din intervalul [4 ; 6] ai byte-ului/caracterului "block13[0]" .
		if (block13[1] > block13[5]) {
			bits_interval = block13[10];
		}
		else {
			bits_interval = block13[15];
		}
		bits_interval = bits_interval << 6 >> 6;		// Obiectul 'bits_interval' va memora bitii 7 si 8 ai unuia dintre bytes: block13[10], block13[15], in bitii 7 si 8 ;
															// restul bitilor ce compun 'bitset'-ul vor avea valoarea '0'.
		block13[0] |= *(char*) &bits_interval;			// Atribuim bitii 7 si 8 ai 'bitset'-ului in bitii 7 si 8 ai byte-ului/caracterului "block13[0]" .
	}
	// Create new byte on index '9':
	const std::bitset<8> bitset_byte15 = block13[14];		// Atunci cand copiem o variabila intr-un obiect de tip 'bitset', bitii variabilei vor ajunge in obiectul de tip 'bitset' in ordinea inversa.
	std::bitset<8> permutated_byte15;
	permutated_byte15[7] = bitset_byte15[5];
	permutated_byte15[6] = bitset_byte15[2];
	permutated_byte15[5] = bitset_byte15[7];
	permutated_byte15[4] = bitset_byte15[4];
	permutated_byte15[3] = bitset_byte15[1];
	permutated_byte15[2] = bitset_byte15[3];
	permutated_byte15[1] = bitset_byte15[0];
	permutated_byte15[0] = bitset_byte15[6];
	block13[9] = *(char*) &permutated_byte15;				// Byte-ul "block13[9]" se va crea din bitii: 3, 6, 1, 4, 7, 5, 8, 2 (in aceasta ordine) ai byte-ului "block13[14]" .
}

void block16_to_block13(std::string& block16) noexcept {	// Decryption.
	unsigned char index;
	for (index = 0; index < 8; ++index) {					// Delete byte: block16[0].
		block16[index] = block16[index + 1];
	}
	for (index = 8; index < 11; ++index) {				// Delete byte: block16[9].
		block16[index] = block16[index + 2];
	}
	block16[11] = block16[14];								// Delete byte: block16[13].
	block16[12] = block16[15];
	block16.resize(13);
}

// Functia urmatoare copiaza bitii din intervalul [LeftByte_LeftBit ; LeftByte_RightBit] al caracterului/byte-ului "Left_Byte"
// in intervalul [RightByte_RightBit - (LeftByte_RightBit - LeftByte_LeftBit) ; RightByte_RightBit] al caracterului/byte-ului "Right_Byte".
static void bits_copy(const char& Left_Byte, const unsigned char& LeftByte_LeftBit, const unsigned char& LeftByte_RightBit, char& Right_Byte, const unsigned char& RightByte_RightBit) noexcept {
	/*
	Intervalele: [LeftByte_LeftBit ; LeftByte_RightBit] si [RightByte_RightBit - (LeftByte_RightBit - LeftByte_LeftBit) ; RightByte_RightBit] trebuie sa:
		-> aibe lungime egala.
		-> sa fie crescatoare (bitul din dreapta intervalului trebuie sa fie mai mare sau egal decat bitul din stanga intervalului).
	*/
	const std::bitset<8> Bitset_Left_Byte = Left_Byte;
	std::bitset<8> Bitset_Right_Byte = Right_Byte;
	const unsigned char& Bitset_LeftByte_LeftBit = 9 - LeftByte_LeftBit;
	unsigned char Bitset_LeftByte_RightBit = 8 - LeftByte_RightBit;
	const char& LeftRange_minus_RightRange = Bitset_LeftByte_RightBit - (8 - RightByte_RightBit);
	for (; Bitset_LeftByte_RightBit < Bitset_LeftByte_LeftBit; ++Bitset_LeftByte_RightBit) {	// Copierea bitilor in "Right_Byte" se va face fara a modifica bitii din afara intervalului stabilit.
		Bitset_Right_Byte[Bitset_LeftByte_RightBit - LeftRange_minus_RightRange] = Bitset_Left_Byte[Bitset_LeftByte_RightBit];
	}
	Right_Byte = *(char*)&Bitset_Right_Byte;													// Copiem cei 8 biti ai 'bitset'-ului in byte-ul/caracterul Right_Byte; se va inversa ordinea bitilor.
}

// Functia urmatoare copiaza al "LeftByte_bit"-lea bit din byte-ul "Left_Byte" in al "RightByte_bit" bit din byte-ul "Right_Byte".
static void bit_copy(const char& Left_Byte, const unsigned char& LeftByte_bit, char& Right_Byte, const unsigned char& RightByte_bit) noexcept {
	const std::bitset<8> Bitset_Left_Byte = Left_Byte;
	std::bitset<8> Bitset_Right_Byte = Right_Byte;
	Bitset_Right_Byte[8 - RightByte_bit] = Bitset_Left_Byte[8 - LeftByte_bit];		// Pozitia (8 - n) din bitset <=> pozitia a n-a din variabila de tip 'char'.
	Right_Byte = *(char*)&Bitset_Right_Byte;
}