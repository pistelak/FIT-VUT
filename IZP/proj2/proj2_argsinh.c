/*
 *    Autor:  Radek Pistelak, xpiste04@stud.fit.vutbr.cz
 *    Rocnik: 2012/2013
 *    Soubor: proj2_argsih.c // obsahuje fce pro vypocet argsinh 
 *    Popis:  Program pomoci iteracnich vypoctu pocita zvolenou funkci
 *            viz. napoveda a dokumentace...
 */

#include <math.h>

#include "proj2_log.h"


/** 
* Fce pocita argsinh, dle vzorce ln (x + sqrt (x^2 + 1))
* Fce vraci hodnotu double. 
*
* Parmetry jsou:
* x -- vstupni hodnota fce
* eps -- pozadovanna presnost
*/

double my_argsinh (double x, double eps)
{
	// pokud je x mensi nez 0 vyuzijeme toho, ze fce argsinh je licha
	// tzn. spocitame vysledek pro abs |x| a vratime -x
	if (x < 0.0) {
		x *= -1;
		x = my_log ((x + sqrt(x*x + 1)), eps);
		return -x;
	}

	// pro 0 rovnou muzeme vratit 0
	else if (x == 0.0) {
		return 0;
	}
	// pro cisla mensi nez 0.01 je zbytecne neefektivni pocitat
	// logaritmus staci nam totiz pouze dva cleny rozvoje
	// x - x^3/6... 
	else if (x < 0.01) {
		return x-((x*x*x)/6); 
	}
	// pro cisla vetsi nez 1e20 zase muzeme ze vzorce zanedbat
	// sqrt (x^2 + 1) jelikoz nam stejne neovlivni vysledek
	else if (x > 1e20){
		return my_log ((2*x), eps);
	}
	else { 
		return my_log ((x + sqrt(x*x + 1)), eps);
	}
}

