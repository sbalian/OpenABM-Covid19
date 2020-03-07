/*
 * utilities.c
 *
 *  Created on: 5 Mar 2020
 *      Author: hinchr
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "constant.h"
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_cdf.h>

/*****************************************************************************************
*  Name:		print_exit
******************************************************************************************/
void print_exit( char *s )
{
    printf("%s\n", s );
    fflush(stdout);
    exit(1);
}

/*****************************************************************************************
*  Name:		print_now
******************************************************************************************/
void print_now( char *s )
{
    printf("%s\n", s );
    fflush(stdout);
}

/*****************************************************************************************
*  Name:		gamma_draw_list
*  Description: generates a draw list so that we can efficiently sample
*  				from a distribution
*
*  Arguments:	list:	pointer to draw list be filled in
*  				n:		length of draw list
*  				mean:	mean of gamma distribution
*  				sd:		sd of gamma distribution
******************************************************************************************/
void gamma_draw_list(
	int *list,
	int n,
	double mean,
	double sd
)
{
	int idx      = 0;
	double a, b;

	b = sd * sd / mean;
	a = mean / b;

	for( idx = 0; idx < n; idx++ )
		list[idx] = ceil( gsl_cdf_gamma_Pinv( ( idx + 1.0 )/( n + 1.0 ), a, b ));
}

/*****************************************************************************************
*  Name:		gamma_rate_curve
*  Description: generates a rate curve for how infectious people are based
*  				upon a discrete gamma distribution and a multiplier
*
*  Arguments:	list:	pointer to draw list be filled in
*  				n:		length of draw list
*  				mean:	mean of gamma distribution
*  				sd:		sd of gamma distribution
*  				factor:	multipler of gamma pdf
******************************************************************************************/
void gamma_rate_curve(
	double *list,
	int n,
	double mean,
	double sd,
	double factor
)
{
	int idx = 0;
	double a, b, total;

	b = sd * sd / mean;
	a = mean / b;

	total = 0;
	for( idx = 0; idx < n; idx++ )
	{
		list[idx] = gsl_cdf_gamma_P( ( idx + 1 ) * 1.0, a, b ) - total;
		total += list[idx];
	}
	for( idx = 0; idx < n; idx++ )
		list[idx] *= factor / total;
}

