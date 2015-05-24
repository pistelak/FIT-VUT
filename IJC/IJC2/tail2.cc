//
//  tail2.cc
//  IJC2
//
//  Created by Radek Pistelak on 28.4.15.
//  Copyright (c) 2015 Radek Pistelak. All rights reserved.
//
//  Email: xpiste04@stud.fit.vutbr.cz
//  Compiler: Apple LLVM version 6.0 (clang-600.0.54) (based on LLVM 3.5svn)
//  Last modified date: 28.4.2015
//
//  -------------------------------------------------------
//
//  Program byl napsan, tak aby co nejverneji kopiroval program
//  psany v jazyku C avsak za pouziti "vymozenosti" C++. 
//

#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>

#include <inttypes.h>

/**
 * @Synopsis  Vytiskne text od n-teho radku
 *
 * @Param is  Zdrojovy stream. 
 * @Param n  Pocet radku, ktere se budou preskakovat.
 */
void print_from_n_line(std::istream& is, uintmax_t n)
{
	std::string line;
       
        // n radku preskakuji
        for ( ; n > 0; --n) {
                std::getline(is, line);
        }
       
        // zbytek vypisu
        while (std::getline(is, line)) {
                std::cout << line << '\n';
        }
}

/**
 * @Synopsis  Vytiskne poslednich n radku.
 *
 * @Param is  Zdrojovy stream.
 * @Param n  Pocet radku, ktere se budou tisknout.
 */
void print_last_n_lines(std::istream& is, uintmax_t n)
{
	if (!is || n == 0) {
		return;
	}

        std::map<uintmax_t, std::string> buffer;

	uintmax_t first, last, read;
	first = last = read = 0;

        // cteni
        for (read = 0; std::getline(is, buffer[last]); read++) {
	
		last = ++last % n;
		if (last == first) {
			first = ++first % n;
		} 
        }
       	
	n = (read < n) ? read : n;

        // vypis
        for (uintmax_t i = 0; i < n; i++) {
                std::cout << buffer[(last + i) % n] << '\n';
        }
}

int main(int argc, const char * argv[]) {
       
        using namespace std;

	ios::sync_with_stdio(false);
        
        bool file = false;              // bude se cist ze souboru?
        const char *file_name;          // nazev souboru
        uintmax_t n = 10;               // n parametr (default je 10)
        bool plus = false;              // plus v parametrech...
       
        
        if (argc == 1) { // default
                
                file = false;
                
        } else if (argc == 2) { // soubor
       
                file = true;
                file_name = argv[1];
                
        } else if (argc == 3 || argc == 4) { // parametry || parametry a soubor
                
                if (argv[1][0] == '-' && argv[1][1] == 'n') {
               
                        const char *num_ptr = argv[2];
                        char *end_ptr;
                        
                        if (*num_ptr == '+') {
                                num_ptr++;
                                plus = true;
                        }
                        
                        n = strtoumax(num_ptr, &end_ptr, 10);
                        
                        if (*end_ptr != '\0') {
                                cerr << "ERR: chybne parametry programu! \n";
                                return EXIT_FAILURE;
                        }
                        
                } else {
                        cerr << "ERR: chybne parametry programu! \n";
                        return EXIT_FAILURE;
                }
                
                if (argc == 4) {
                        file_name = argv[3];
                        file = true;
                } else {
                        file = false;
                }
        }
        
        ifstream fp;
        string line_buffer;
       
        if (file) {
                fp.open(file_name);
                if (!fp.is_open()) {
                        cerr << "ERR: nelze otevrit soubor" << file_name << '\n';
                        return EXIT_FAILURE;
                }
        }
      
        if (plus) {
                print_from_n_line((file) ? fp : cin, n);
        } else {
                print_last_n_lines((file) ? fp : cin, n);
        }
        
        return 0;
}

// End of file: tail2.cc

