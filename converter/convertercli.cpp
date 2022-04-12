#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <regex>
#include <map>
#include <stdexcept>
#include "converters.hpp"

namespace CommandLineParser {
	struct Argument {
		const char* value;
		const char* help_string;
		const char* type;
		
		Argument(const char* inp_value, const char* inp_help_string, const char* inp_type) {
			value = inp_value;
			help_string = inp_help_string;
			type = inp_type;
		}

		Argument() {}

		auto convert(){
			if(type == "bool") {
				return (bool)value;
			}
			else if(type == "str") {
				return value;
			}
			else if(type == "int") {
				return atoi(value);
			}
		}
	};
	class ArgumentParser {
		public:
			void AddArgument(const char* argument_name, const char* help_string = "", const char* type = "str", bool store_true = false) {
				if(store_true == true) {
					options.insert(std::pair<const char*, Argument> {argument_name, ("1", help_string, "bool")});	
				}
			}
			void ParseArguments(int argc, char** argv) {
				for (size_t i = 1; i < argc; i++) {

				}
			}
			void PrintHelpText() {

			}

			std::map<const char*, Argument> options;
			std::vector<const char*, Argument> positional_args;

		private:
			void m_RaiseError(const char* error_message) {
				std::cout << error_message;
				throw std::exception;
			}
	};
}

int main(int argc, char** argv) {
	using namespace CommandLineParser;
	ArgumentParser parser;
	parser.AddArgument("input", "Format of the input file")
}
