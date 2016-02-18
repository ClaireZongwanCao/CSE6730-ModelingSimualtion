//
//  RandNumTest.h
//  test
//
//  Created by Yaohong Xi on 2/19/15.
//  Copyright (c) 2015 xiyaohong. All rights reserved.
//

#ifndef test_RandNumTest_h
#define test_RandNumTest_h
#include <fstream>
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <iomanip>
#include <stdio.h>
#include <math.h>

// Generate a random from uniform distrubution with expectation (max+min)/2 and variance (max-min)*(max-min)/12
double randUniform(double min, double max)
{
    double x = (double)rand() / RAND_MAX;
    x = x*(max-min)+min;
    return x;
}

// Generate a random from Normal distrubution, with the expectation e and variance v
// reference Marsaglia and Bray (1964)
double randNormal(double e, double v)
{
    static double v1, v2, s;
    static int phase = 0;
    double x;
    x = x * v + e;
    
    if ( phase == 0 ) {
        do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;
            
            v1 = 2 * U1 - 1;
            v2 = 2 * U2 - 1;
            s = v1 * v1 + v2 * v2;
        } while(s >= 1 || s == 0);
        
        x = v1 * sqrt(-2 * log(s) / s);
    } else
        x = v2 * sqrt(-2 * log(s) / s);
    
    phase = 1 - phase;
    
    return x;
}

// Generate a random from uniform distrubution with expectation Lamda and variance Lamda
double randPoisson(double Lamda)
{
    double x=0,b=1,c=exp(-Lamda),u;
    while (b>=c) {
        u=(double)rand() / RAND_MAX;
        b *=u;
        if(b>=c) x++;
    }
    return x;
}

// Generate a random from Chi square distrubution with expectation k and variance 2k
double randX2(double k)
{
    double x = 0.0;
    for (int i=0;i<k;i++)
    {
        double p_dv = randNormal(0.0,1.0);
        x += p_dv*p_dv;
    }
    return x;
}

#endif
