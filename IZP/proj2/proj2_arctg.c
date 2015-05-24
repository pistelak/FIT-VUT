/*
 *    Autor:  Radek Pistelak, xpiste04@stud.fit.vutbr.cz
 *    Rocnik: 2012/2013
 *    Soubor: proj2_arctg.c // obsahuje fce pro vypocet arctg. 
 *    Popis:  Program pomoci iteracnich vypoctu pocita zvolenou funkci
 *            viz. napoveda a dokumentace...
 */

#include <math.h>
#include <stdbool.h>

#define PI_2 1.5707963267948966192313
#define PI_4 0.7853981633974483096156

#define ATAN_0_1 0.0996686524911620273784461
#define HELP_VALUE -0.1 

/** 
* Taylorova rada pro vypocet arctg 
* x - x^3/3 + x^5/5 ... 
* Fce vraci typ double.
* Jako parametry potrebuje:
* x -- vstupni hodnota fce 
* eps -- pozadovana presnost
*/

double my_arctg_series (double x, double eps)
{
	double sum = x;
	double x_2 = x*x;
	double x_old = x; 
	double x_new = x; 
	int k = 1; 

	while (fabs(x_new) > eps)
	{
		k += 2;
		x_old *= -(x_2); 
		x_new = x_old / k;
		sum += x_new;
	} 

	return sum;
}

/**
* Fce slouzi ke zuzeni def. oboru, tak aby byl vhodny
* pro konkretni taylorvu radu.
*
* Vraci double. 
* Jako parametry potrebuje:
* x -- vstupni hodnota fce 
* eps -- pozadovana presnost
*/

double my_arctg (double x, double eps)
{
	bool negative = false; 

	// pokud je x mensi nez 0 vyuzijeme toho, ze fce argsinh je licha
	// tzn. spocitame vysledek pro abs |x| a vratime -x
	if (x < 0.0)
	{
		x *= -1; 
		negative = true; 
	}

	// pokud je x vetsi nez 1 vysledek ziskame, tak ze 
	// taylorove rade predame prevracenou hodnotu x, 
	// kterou nasledne odecteme od PI/2 
	if (x > 1.0)  
	{
		x = 1/x;
		x = my_arctg_series (x, eps); 
		x = PI_2 - x;
	}

	// zvolena taylorova rada spatne konverguje, v intervalu 
	// od 0.8 do 1.0. -> vyuzijeme tedy, ze plati
	// arctan (x) + arctan (y) = arctan (x+y / 1 - x*y)
	else if (x > 0.8 && x <= 1.0)
	{  
		const double y = HELP_VALUE;
		x = ((x+y) / (1 - (x*y))); 
		x = my_arctg_series(x, eps);
		x -= (-ATAN_0_1); 
	}
	else 
	{
		x = my_arctg_series (x, eps);
	}

	return (negative == true) ? -x : x; 
}


