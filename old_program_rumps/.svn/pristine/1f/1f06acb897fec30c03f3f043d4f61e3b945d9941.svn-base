//------------------------------------------------------------------------------
// Library
// Name				: comm_testlib.h
// Purpose
// - Provides function calls related to communication system testing
// such as noise generator, etc.

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.1415926536

// Generates normal distribution random number with 'mu' mean and variance of 'sigma'
// by Phoxis
// https://phoxis.org/2013/05/04/generating-random-numbers-from-normal-distribution-in-c/
double
randn (double mu, double sigma)
{
  double U1, U2, W, mult;
  static double X1, X2;
  static int call = 0;
 
  if (call == 1)
    {
      call = !call;
      return (mu + sigma * (double) X2);
    }
 
  do
    {
      U1 = -1 + ((double) rand () / RAND_MAX) * 2;
      U2 = -1 + ((double) rand () / RAND_MAX) * 2;
      W = pow (U1, 2) + pow (U2, 2);
    }
  while (W >= 1 || W == 0);
 
  mult = sqrt ((-2 * log (W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;
 
  call = !call;
 
  return (mu + sigma * (double) X1);
}

// Generates additive white Gaussian Noise samples with zero mean and a standard deviation of 1. 
// ** still buggy, sometimes it produces #IND exception
// by Dr Cagri Tanriover
// https://www.embeddedrelated.com/showcode/311.php
double AWGN_generator()
{
 
  double temp1;
  double temp2;
  double result;
  int p;

  p = 1;

  while( p > 0 )
  {
	temp2 = ( rand() / ( (double)RAND_MAX ) ); /*  rand() function generates an
                                                       integer between 0 and  RAND_MAX,
                                                       which is defined in stdlib.h.
                                                   */

    if ( temp2 == 0 )
    {// temp2 is >= (RAND_MAX / 2)
      p = 1;
    }// end if
    else
    {// temp2 is < (RAND_MAX / 2)
       p = -1;
    }// end else

  }// end while()

  temp1 = cos( ( 2.0 * (double)PI ) * rand() / ( (double)RAND_MAX ) );
  result = sqrt( -2.0 * log( temp2 ) ) * temp1;

  return result;	// return the generated random sample to the caller

}// end AWGN_generator()


