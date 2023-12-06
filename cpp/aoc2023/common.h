#pragma once

using namespace std;

#include <vector>
#include <string>

std::vector<std::string> read_input_file(const char* file_name);

vector<string> split_delim(const string& line, char delimiter);
