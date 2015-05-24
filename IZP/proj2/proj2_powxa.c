/*
 *    Autor:  Radek Pistelak, xpiste04@stud.fit.vutbr.cz
 *    Rocnik: 2012/2013
 *    Soubor: proj2_powxa.c // obsahuje fce pro vypocet mocniny x^a. 
 *    Popis:  Program pomoci iteracnich vypoctu pocita zvolenou funkci
 *            viz. napoveda a dokumentace...
 */

#include <math.h>
#include <stdbool.h>

#include "proj2_log.h"
#include "proj2_exp.h"

double my_powxa (double x, double a, double eps)
{
	if (a == 0.0)
		return 1.0;
	else if (x == 0.0 && a < 0.0)
		return INFINITY;
	else if (x == 1.0)
		return 1.0;  // jedna na cokoli je porad jedna
	else if (isinf (x) && a == 0.0)
		return 1.0; // cokoli na nultou je jedna 
	else if (isinf(x) && a > 0.0)
		return INFINITY; // nekonecno
	else if (isinf(x) && a < 0.0)
		return 0.0;
	else if (x == 0.0) // nula na cokoli je porad nula
		return 0;
	
	bool negative_a = false;
	bool negative_x = false; 

	if (a < 0.0 && x > 0.0)
	{  // 1/x^a
		negative_a = true;
		a *= -1;
	}
	else if (a > 0.0 && x < 0.0)
	{  // zjisteni zda je vysledek kladny nebo zaporny
	   // sudy exponent == kladny vysledek a naopak
		negative_x = true; 
		x *= -1;
	}
	else if (a < 0.0 && x < 0.0)
	{
		// zaporne x a zaporny exponent --> zaporny vysledek
		// - 1/ x^a
		negative_a = true; 
		negative_x = true;
		x *= -1;
		a *= -1;
	}

	double x_log = my_log (x, eps);
	double x_exp;

	if (x_log < 0.0)
	{
		x_log *= -1; 
		x_exp = my_exp ((a*x_log), eps);
		x_exp = 1/x_exp;
	}
	else 
	{
		x_exp = my_exp ((a*x_log), eps);
	}

	if (negative_a == true && negative_x == false) {
		return 1/x_exp; 	 
	}
	else if (negative_a == false && negative_x == true) {
		return ((((int)a) % 2)== 0) ? x_exp : -x_exp;
	}
	else if (negative_a == true && negative_x == true) {
		return - (1/x_exp); 
	} 
	else 
		return x_exp; 
}

