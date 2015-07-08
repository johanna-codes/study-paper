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
 

#include "grassmann-metrics-def.hpp"
#include "grassmann-metrics-impl.hpp"
 
#include "kth-LogEucl-svm-cross-validation-def.hpp"
#include "kth-LogEucl-svm-cross-validation-impl.hpp"

#include "kth-SteinDiv-svm-cross-validation-def.hpp"
#include "kth-SteinDiv-svm-cross-validation-impl.hpp"
 
#include "kth-GrassPM-svm-cross-validation-def.hpp"
#include "kth-GrassPM-svm-cross-validation-impl.hpp"

 
#include "kth-GrassBC-svm-cross-validation-def.hpp"
#include "kth-GrassBC-svm-cross-validation-impl.hpp"


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
  int p = 12;//PQ NO SE> POR BOBA> FUE UN ERROR :(
  
  field<string> all_people;
  all_people.load(peopleList);
  
  //Cross Validation LogEuclidean
//   cout << "Cross Validation for Log-Euclidean Distance" << endl;
//   kth_cv_svm_LogEucl run_kth_cv_svm_LogEucl(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
//   run_kth_cv_svm_LogEucl.run();
   
//    //Cross Validation Stein Divergence
//   cout << "Cross Validation for Stein Divergence" << endl;
//   kth_cv_svm_Stein run_kth_cv_svm_SD(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
//   run_kth_cv_svm_SD.run();
  
//   //Cross Validation Grassmann Projection Metric
//   cout << "Cross Validation for Grassmann: PM" << endl;
//   kth_cv_svm_Grass_PM run_kth_cv_svm_PM(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
//   run_kth_cv_svm_PM.run(p);
  
//   //Cross Validation Grassmann Binet-Cauchy Metric
  cout << "Cross Validation for Grassmann: BC" << endl;
  kth_cv_svm_Grass_BC run_kth_cv_svm_BC(path, actionNames, all_people, scale_factor, shift, total_scenes,  dim);
  run_kth_cv_svm_BC.run();
   
   
   //kth_CV_omp_onesegment.SteinDiv();
   //kth_CV_omp_onesegment.proj_grass(p);
   //kth_CV_omp_onesegment.BC_grass();
   
   
  
  
  
  
  return 0;
  
}
