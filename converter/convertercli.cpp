#include <iostream>
#include <regex>
#include <fstream>
#include <string>
#include "converters.hpp"

void RaiseError(const char* error_message) {
	std::cout << error_message;
	std::exit(1);
}

int main(int argc, char** argv) {
	std::ifstream input_file("in.txt");
	std::ofstream output_file("out.txt");
	
	std::string input_format = argv[1];
	std::string output_format = argv[2];

	if(input_format == "van") {
		if(output_format == "rle") {
			vanToRle(input_file, output_file);
			return 0;
		}
		else if(output_format == "ptxt") {
			vanToPtxt(input_file, output_file);
			return 0;
		}
		RaiseError("Invalid output format. Options for this input format: rle, ptxt.");
	}
	else if(input_format == "rle") {
		if(output_format == "van") {
			rleToVan(input_file, output_file);
			return 0;
		}
		else if(output_format == "ptxt") {
			rleToPtxt(input_file,  output_file);
			return 0;
		}
		RaiseError("Invalid output format. Options for this input formant: van, ptxt.");
	}
	else if(input_format == "ptxt") {
		if(output_format == "van") {
			ptxtToVan(input_file, output_file);
			return 0;
		}
		else if(output_format == "rle") {
			ptxtToRle(input_file, output_file);
			return 0;
		}
		RaiseError("Invalid output format. Options for this input format: van, rle.");
	}
	else if(input_format == "mul") {
		if(output_format == "van") {
			mulToVan(input_file, output_file);
			return 0;
		}
		RaiseError("mul format can only be converted to van format.");
	}
	RaiseError("Invalid input format. Options: van, rle, ptxt, mul.");
}
