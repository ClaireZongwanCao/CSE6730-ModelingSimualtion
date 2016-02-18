//
//  main.cpp
//  test
//
//  Created by Yaohong Xi on 2/12/15.
//  Copyright (c) 2015 xiyaohong. All rights reserved.
//

#include  "RandNumTest.h"

using namespace std;


int main(int argc, const char * argv[]) {
    srand( (unsigned int) time(NULL) );
    int m = 1000000;
    ofstream randU;
    randU.open ("/Users/xiyaohong/Desktop/randUniform.txt");
    
    for (int i = 0; i<m; i++){
        double tempRand =randUniform(0.0, 1.0);
        randU << tempRand<<endl;
        //cout<<randUniform(0.0, 1.0)<<endl;
        
    }
    randU.close();
    
    
    ofstream randN;
    randN.open ("/Users/xiyaohong/Desktop/randNormal.txt");
    
    for (int i = 0; i<m; i++){
        double tempRand =randNormal(1000.0, 200.0);
        randN << tempRand<<endl;
        
    }
    randN.close();
    
    
//    ofstream randP;
//    randP.open ("/Users/xiyaohong/Desktop/randPoisson.txt");
//    
//    for (int i = 0; i<m; i++){
//        double tempRand =randPoisson(1000.0);
//        randP << tempRand<<endl;
//
//        
//    }
//    randP.close();
    
//    ofstream randX2;
//    randN.open ("/Users/xiyaohong/Desktop/randX2.txt");
//    
//    for (int i = 0; i<m; i++){
//        double tempRand = randX2(1000.0);
//        randX2 << tempRand<<endl;
//        //cout<<randUniform(0.0, 1.0)<<endl;
//        //cout<<tempRand<<endl;
//        
//    }
//    randX2.close();
    
    
    
}

