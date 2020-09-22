#include <string>

void block10_to_block13(std::string& block10) noexcept {	// Encryption.
	{
		block10.resize(13);									// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, NULL, NULL, NULL
		unsigned char index;
		for (index = 12; index > 9; --index) {
			block10[index] = block10[index - 3];			// 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 7, 8, 9	; index-ul 9 devine NULL
		}
		for (index = 8; index > 5; --index) {
			block10[index] = block10[index - 2];			// 0, 1, 2, 3, 4, 5, 4, 5, 6, 9, 7, 8, 9	; index-ul 5 devine NULL
		}
		for (index = 4; index > 2; --index) {
			block10[index] = block10[index - 1];			// 0, 1, 2, 2, 3, 5, 4, 5, 6, 9, 7, 8, 9	; index-ul 2 devine NULL
		}
	}															// result: 0, 1, NULL, 2, 3, NULL, 4, 5, 6, NULL, 7, 8, 9
	unsigned char* const Unsigned_block10 = (unsigned char*) &block10[0];
	Unsigned_block10[2] = (Unsigned_block10[8] ^ Unsigned_block10[10]) + Unsigned_block10[7] & Unsigned_block10[11];	// Create first byte in memory: block10[2].
	Unsigned_block10[5] = (Unsigned_block10[12] | Unsigned_block10[1]) & Unsigned_block10[0];									// Create second byte in memory: block10[5].
	Unsigned_block10[9] = (Unsigned_block10[6] ^ Unsigned_block10[4]) - Unsigned_block10[3];									// Create third byte in memory: block10[9].
}

void block13_to_block10(std::string& block13) noexcept {	// Decryption.
	unsigned char index;
	for (index = 2; index < 4; ++index) {					// Delete byte: block10[2].
		block13[index] = block13[index + 1];
	}
	for (index = 4; index < 7; ++index) {					// Delete byte: block10[5].
		block13[index] = block13[index + 2];
	}
	for (index = 7; index < 10; ++index) {				// Delete byte: block10[9].
		block13[index] = block13[index + 3];
	}
	block13.resize(10);
}