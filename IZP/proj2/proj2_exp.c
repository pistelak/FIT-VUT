/*
 *    Autor:  Radek Pistelak, xpiste04@stud.fit.vutbr.cz
 *    Rocnik: 2012/2013
 *    Soubor: proj2_exp.c // obsahuje fce pro vypocet exp. 
 *    Popis:  Program pomoci iteracnich vypoctu pocita zvolenou funkci
 *            viz. napoveda a dokumentace...
 */

#include <math.h>
#include <float.h>


/**
* Nekonecna rada pro vypocet exponentu
* 
* Vraci double. 
* Pozaduje vstupni hodnotu x a presnost eps.
*/
double my_exp_series (double x, double eps)
{
	double sum;
	double k; 
	double t; 

	sum = 1 + x;
	k = 1;
	t = x;

	while (fabs(t) > eps)
	{
		k += 1; 
		t *= x/k;
		sum += t;
		if (sum > DBL_MAX) // cisla zde rychle narustaji tak kontrolujeme velikost 
			return INFINITY; // zda uz neni vetsi nez se vleze do naseho dat. typu..
	}
	
	return sum;

}

/**
* Fce jen vola my_exp_series. 
* Pro dodrzeni stylu hlavickovych souboru.
*
*/
double my_exp (double x, double eps)
{
	return my_exp_series (x, eps); 
}
