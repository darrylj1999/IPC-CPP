// This script makes an average histogram across multiple files
#include <map>
#include <limits>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>

int main(int argc, char** argv) {
	std::string line("");
	std::map< std::string, double > data;
	float weight = 1. / ( argc - 1 );

	for ( int i = 1; i < argc; i++ ) {
		std::ifstream infile( argv[i] );
		while ( std::getline( infile, line ) ) {
			std::size_t last_space_index = line.find_last_of(' ');
			std::string key = line.substr( 0, last_space_index );
			std::string str_val = line.substr( last_space_index + 1 );
			data[key] += atof( str_val.c_str() ) * weight; 
		}
	}

	std::cout << std::setprecision( std::numeric_limits< double >::max_digits10 );
	for ( std::map< std::string, double >::iterator it = data.begin(); it != data.end(); ++it ) {
		std::cout << it -> first << ' ' << it -> second << std::endl;
	}
}
