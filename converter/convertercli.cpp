#include <cxxopts.hpp>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <regex>
#include "converters.hpp"

int main(int argc, char** argv){
	cxxopts::Options options(argv[0], "A command line tool to use GoL converters with.");
	options.add_options()
		("h,help", "Print Usage")
		("p,pout", "Program outputs to input.txt if selected. Otherwise it will default to out.txt")
		("o,output", "Output Format: ptxt(plaintext), rle, van(vanilla)", cxxopts::value<std::string>())
		("i,inpformat", "Input Format: ptxt(plaintext), rle, van(vanilla), mul(multi)", cxxopts::value<std::string>())
	;
	auto result = options.parse(argc, argv);
	if(result.count("help")) std::cout << options.help();

	std::ifstream input("in.txt");
	std::string dir;
	if(result.count("pout")) dir = "..\\input.txt";
	else dir = "out.txt";
	std::ofstream output(dir);

	if(result.count("output") && result.count("inpformat")){
		if(result["inpformat"].as<std::string>() == "ptxt"){
			if(result["output"].as<std::string>() == "rle"){
				std::cout << "Converter not available yet. Please convert to van first.";
				return 0;
			}
			else if(result["output"].as<std::string>() == "van") ptxtToVan(input, output);
			else{
				std::cout << "This type '" << result["output"].as<std::string>() << "' is not defined for this conversion.";
				return 0;
			}
		}
		else if(result["inpformat"].as<std::string>() == "rle"){
			if(result["output"].as<std::string>() == "ptxt"){
				std::cout << "Converter not available yet. Please convert to van first.";
				return 0;
			}
			else if(result["output"].as<std::string>() == "van") rleToVan(input, output);
			else{
				std::cout << "This type '" << result["output"].as<std::string>() << "' is not defined for this conversion.";
				return 0;
			}
		}
		else if(result["inpformat"].as<std::string>() == "van"){
			if(result["output"].as<std::string>() == "ptxt") vanToPtxt(input, output);
			else if(result["output"].as<std::string>() == "rle") vanToRle(input, output);
			else{
				std::cout << "This type '" << result["output"].as<std::string>() << "' is not defined for this conversion.";
				return 0;
			}
		}
		else if(result["inpformat"].as<std::string>() == "mul"){
			if(result["output"].as<std::string>() == "ptxt" || result["output"].as<std::string>() == "rle"){
				std::cout << "Converter not available yet. Please convert to van first.";
				return 0;
			}
			else if(result["output"].as<std::string>() == "van") mulToVan(input, output);
			else{
				std::cout << "This type '" << result["output"].as<std::string>() << "' is not defined for this conversion.";
				return 0;
			}
		}
		else{
			std::cout << "This type '" << result["inpformat"].as<std::string>() << "' is not defined.";
			return 0;
		}
	}
	else{
		input.close(); output.close();
		return 0;
	}
	input.close(); output.close();
}