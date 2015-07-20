#include <omp.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <armadillo>
#include <iomanip>
#include <vector>
#include <hdf5.h>
#include "omp.h"

 using namespace std;
 using namespace arma;
 
// #include "aux-functions-def.hpp"
// #include "aux-functions-impl.hpp"

// #include "riemannian-metrics-def.hpp"
// #include "riemannian-metrics-impl.hpp"

#include "grassmann-metrics-def.hpp"
#include "grassmann-metrics-impl.hpp"
 
#include "kth-cross-validation-one-def.hpp"
#include "kth-cross-validation-one-impl.hpp"




//Home
//const std::string path = "/media/johanna/HD1T/codes/datasets_codes/KTH/"; 

//WANDA
const std::string path  = "/home/johanna/codes/codes-git/study-paper/trunk/";



const std::string peopleList = "people_list.txt";
const std::string  actionNames = "actionNames.txt";

///kth
// int ori_col = 160;
// int ori_row = 120;





int
main(int argc, char** argv)
{
  
   if(argc < 3 )
  {
    cout << "usage: " << argv[0] << " scale_factor " << " shift_factor " << endl;
    return -1;
  }
  
  
  int scale_factor = atoi( argv[1] );
  int shift = atoi( argv[2] );
  int total_scenes = 1; //Only for Scenario 1.
  int dim = 14; 
  //int p = 12;//PQ NO SE> POR BOBA> FUE UN ERROR :(
  
  field<string> all_people;
  all_people.load(peopleList);
  
  
  //Cross Validation
//    kth_cv_omp kth_CV_omp_onesegment(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
//    kth_CV_omp_onesegment.logEucl();
//    kth_CV_omp_onesegment.SteinDiv();
//    kth_CV_omp_onesegment.proj_grass(p);

   
    
  vec vec_bc = zeros(dim);
  vec vec_pm = zeros(dim);
  for (int p=1; p<= dim; ++p)
  {
    cout << "p= " << p << endl;
    kth_cv_omp kth_CV_omp_onesegment(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
    vec_pm(p-1) = kth_CV_omp_onesegment.proj_grass(p);
    vec_bc(p-1) = kth_CV_omp_onesegment.BC_grass(p);
  }
  
  vec_pm.t().print("Projection Metric");
  vec_bc.t().print("Binet-Cauchy");
  
  
  
  
  return 0;
  
}
