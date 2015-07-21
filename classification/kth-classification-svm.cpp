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
  
  
  
  
  int total_scenes = 1; //Only for Scenario 1.
  int dim = 14; 
  //int p = 12;//PQ NO SE> POR BOBA> FUE UN ERROR :(
  
  field<string> all_people;
  all_people.load(peopleList);
  
  
  //********************************************************************************
  // ******************************Training****************************************** 
  //********************************************************************************  
  //int scale_factor = 1;
  //int shift = 0; 
  //Cross Validation LogEuclidean
  //         cout << "Training Log-Euclidean Distance" << endl;
  //         kth_cv_svm_LogEucl run_kth_cv_svm_LogEucl(path, actionNames, all_people, total_scenes,  dim);
  //         run_kth_cv_svm_LogEucl.train(scale_factor, shift);
  //         
  //          //Cross Validation Stein Divergence
  //         cout << "Training Stein Divergence" << endl;
  //         kth_cv_svm_Stein run_kth_cv_svm_SD(path, actionNames, all_people, total_scenes,  dim);
  //         run_kth_cv_svm_SD.train(scale_factor, shift);
  
  
  //vec vec_bc = zeros(dim);
  //vec vec_pm = zeros(dim);
  
  //OJO: los dist_vec no se guardan de acuerdo a p!!!!!!!!!!!!!!!!!!!!!!
  
  //for (int p=1; p<=dim; ++p )
  //   {
  //     //Cross Validation Grassmann Projection Metric
  //     //int best_p_PM  = 8;
  //     cout << "Training for Grassmann PM, p=  " << p << endl;
  //     kth_cv_svm_Grass_PM run_kth_cv_svm_PM(path, actionNames, all_people, total_scenes,  dim);
  //     run_kth_cv_svm_PM.train(best_p_PM, scale_factor, shift);
  //     vec_pm(p-1) = run_kth_cv_svm_PM.test(best_p_PM, scale_factor, shift);
  //     
  //      //Cross Validation Grassmann Binet-Cauchy Metric
  //     //best_p_BC = 9;
  //     cout << "Training Grassmann with Binet-Cauchy metric, p= " << 9 << endl;
  //     kth_cv_svm_Grass_BC run_kth_cv_svm_BC(path, actionNames, all_people, total_scenes,  dim);
  //     run_kth_cv_svm_BC.train(best_p_BC, scale_factor, shift); 
  //     vec_bc(p-1) = run_kth_cv_svm_BC.test(best_p_BC, scale_factor, shift);
  //   }
  
  //vec_pm.t().print("Projection Metric");
  //vec_bc.t().print("Binet-Cauchy");
  
  //********************************************************************************
  // ******************************Testing****************************************** 
  //********************************************************************************
  
  int best_p_PM = 8;
  int best_p_BC = 9;
  vec vec_shift;
  vec_shift << -25 << -20 << -15 << -10 << -5 << 5 << 10 << 15 << 20 << 25 << endr;
  int scale_factor =1;
  
  for (int i=0; i< vec_shift.n_elem; ++i)
  {
    
    int shift = vec_shift(i);
    cout << "Shift: " << shift << endl;
    
//     cout << "Cross Validation for Log-Euclidean Distance" << endl;
//     kth_cv_svm_LogEucl run_kth_cv_svm_LogEucl(path, actionNames, all_people, total_scenes,  dim);
//     run_kth_cv_svm_LogEucl.test(scale_factor, shift);
//     
//     cout << "Cross Validation for Stein Divergence" << endl;
//     kth_cv_svm_Stein run_kth_cv_svm_SD(path, actionNames, all_people, total_scenes,  dim);
//     run_kth_cv_svm_SD.test(scale_factor, shift);
    
    cout << "Cross Validation for Grassmann using Projection Metric" << endl;
    kth_cv_svm_Grass_PM run_kth_cv_svm_PM(path, actionNames, all_people, total_scenes,  dim);
    run_kth_cv_svm_PM.test(best_p_PM, scale_factor, shift);
    
    cout << "Cross Validation for Grassmann using Binet-Cauchy metric" << endl;
    kth_cv_svm_Grass_BC run_kth_cv_svm_BC(path, actionNames, all_people, total_scenes,  dim);
    run_kth_cv_svm_BC.test(best_p_BC, scale_factor, shift);
    cout << "********************************************************************************"<< endl;
    
  }
  
  
  
  
  return 0;
  
}
