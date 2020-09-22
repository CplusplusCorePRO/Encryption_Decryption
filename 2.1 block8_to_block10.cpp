#include <string>

void block8_to_block10(std::string& block8) noexcept {	// Encryption.
	{
		block8.resize(10);
		unsigned char index;
		for (index = 9; index > 6; --index) {			// Bytes se permuta la dreapta (de la index mic la index mare) astfel incat noii bytes se vor crea
			block8[index] = block8[index - 2];		// pe index-ul '1' si indexul '6' al string-ului care contine 10 caractere/bytes.
		}
		for (index = 5; index > 1; --index) {
			block8[index] = block8[index - 1];
		}
	}
	const unsigned char* const Unsigned_block8 = (unsigned char*) &block8[0];
	{	// Create new byte on index '1':
		const unsigned char& bytes_addition = Unsigned_block8[3] + Unsigned_block8[4] + Unsigned_block8[7] + Unsigned_block8[9];
		if (bytes_addition > 127) {
			block8[1] = (Unsigned_block8[4] | Unsigned_block8[9]) & bytes_addition;
		}
		else {
			block8[1] = Unsigned_block8[3] & Unsigned_block8[7] | bytes_addition;
		}
	}
	// Create new byte on index '6':
	block8[6] = 0;
	unsigned char positive_byte = Unsigned_block8[0];
	positive_byte >>= 2;
	positive_byte <<= 6;
	positive_byte >>= 4;
	block8[6] |= positive_byte;					// Bitii: 5 si 6 ai primului byte din string-ul de 10 caractere se vor atribui pe bitii 5 si 6 ai lui "block8[6]" .
	positive_byte = Unsigned_block8[2];
	positive_byte <<= 2;
	positive_byte >>= 6;
	positive_byte <<= 4;
	block8[6] |= positive_byte;					// Bitii: 3 si 4 ai celui de-al treilea byte din string-ul de 10 caractere se vor atrebui pe bitii 3 si 4 ai lui "block8[6]" .
	positive_byte = Unsigned_block8[5];
	positive_byte <<= 6;
	positive_byte >>= 6;
	block8[6] |= positive_byte;					// Bitii: 7 si 8 ai celui de-al 6-lea byte din string-ul de 10 caractere se vor atribui pe bitii 7 si 8 ai lui "block8[6]" .
	positive_byte = Unsigned_block8[8];
	positive_byte >>= 6;
	positive_byte <<= 6;
	block8[6] |= positive_byte;					// Bitii: 1 si 2 ai celui de-al 9-lea byte din string-ul de 10 caractere se vor atribui pe bitii 1 si 2 ai lui "block8[6]" .
}

void block10_to_block8(std::string& block10) noexcept {	// Decryption.
	unsigned char index;
	for (index = 1; index < 5; ++index) {				// Delete byte: block10[1].
		block10[index] = block10[index + 1];
	}
	for (index = 5; index < 8; ++index) {				// Delete byte: block10[6].
		block10[index] = block10[index + 2];
	}
	block10.resize(8);
}