#include <string>

static inline void column_to_line(const std::string& block16, std::string& permutated_matrix, unsigned char& index_permutated_matrix, unsigned char column) noexcept;

void matrix_permutation(std::string& block16) noexcept {			// Encryption.
	std::string permutated_matrix;								// string-ul/block-ul 'block16' va fi vazut ca o matrice patratica cu 4 linii si 4 coloane.
	permutated_matrix.resize(16);
	unsigned char index_permutated_matrix = 0;
	column_to_line(block16, permutated_matrix, index_permutated_matrix, 1);			// Coloana 2 va fi retinuta in memoria liniei 1.
	column_to_line(block16, permutated_matrix, index_permutated_matrix, 0);			// Coloana 1 va fi retinuta in memoria liniei 2.
	column_to_line(block16, permutated_matrix, index_permutated_matrix, 3);			// Coloana 4 va fi retinuta in memoria liniei 3.
	column_to_line(block16, permutated_matrix, index_permutated_matrix, 2);			// Coloana 3 va fi retinuta in memoria liniei 4.
	block16 = permutated_matrix;								// Noul block/string obtinut NU reprezinta matricea transpusa a vechiului block/string.
}

static inline void line_to_column(const std::string& block16, std::string& reverse_permutated_matrix, unsigned char& index_reverse_permutated_matrix, unsigned char column) noexcept;

void reverse_matrix_permutation(std::string& block16) noexcept{		// Decryption.
	std::string reverse_permutated_matrix;
	reverse_permutated_matrix.resize(16);
	unsigned char index_reverse_permutated_matrix = 0;
	line_to_column(block16, reverse_permutated_matrix, index_reverse_permutated_matrix, 1);	// Linia 1 va fi retinuta in memoria coloanei 2.
	line_to_column(block16, reverse_permutated_matrix, index_reverse_permutated_matrix, 0);	// Linia 2 va fi retinuta in memoria coloanei 1.
	line_to_column(block16, reverse_permutated_matrix, index_reverse_permutated_matrix, 3);	// Linia 3 va fi retinuta in memoria coloanei 4.
	line_to_column(block16, reverse_permutated_matrix, index_reverse_permutated_matrix, 2);	// Linia 4 va fi retinuta in memoria coloanei 3.
	block16 = reverse_permutated_matrix;
}

static inline void column_to_line(const std::string& block16, std::string& permutated_matrix, unsigned char& index_permutated_matrix, unsigned char column) noexcept {
	for (; column < 16; column += 4) {
		permutated_matrix[index_permutated_matrix] = block16[column];
		++index_permutated_matrix;
	}
}

static inline void line_to_column(const std::string& bock16, std::string& reverse_permutated_matrix, unsigned char& index_reverse_permutated_matrix, unsigned char column) noexcept {
	for (; column < 16; column += 4) {
		reverse_permutated_matrix[column] = bock16[index_reverse_permutated_matrix];
		++index_reverse_permutated_matrix;
	}
}