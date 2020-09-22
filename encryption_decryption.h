#pragma once
#include <string>

bool file_encrypt(const std::string& input_file_name, const std::string& output_file_name, const std::string& key_16bytes, std::string initialization_vector) noexcept;
bool file_decrypt(const std::string& input_file_name, const std::string& output_file_name, const std::string& key_16bytes, std::string initialization_vector) noexcept;
