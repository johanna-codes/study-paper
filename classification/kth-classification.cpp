#include <omp.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <armadillo>
#include <iomanip>
#include <vector>
#include <hdf5.h>


 using namespace std;
 using namespace arma;
 
// #include "aux-functions-def.hpp"
// #include "aux-functions-impl.hpp"

// #include "riemannian-metrics-def.hpp"
// #include "riemannian-metrics-impl.hpp"
 
#include "kth-cross-validation-def.hpp"
#include "kth-cross-validation-impl.hpp"




//Home
//const std::string path = "/media/johanna/HD1T/codes/datasets_codes/KTH/"; 

//WANDA
const std::string path = "/home/johanna/codes/codes-git/study-paper/trunk/cov_matrices";



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
  int total_scenes = 4;
  int segment_length = 20;
  int dim = 12; //Action Recognition from Video Using feature Covariance Matrices
  
  field<string> all_people;
  all_people.load(peopleList);
  
   kth_cv kth_CV(path, actionNames, all_people, scale_factor, shift, total_scenes, segment_length, dim);
   //kth_CV.logEucl();
   kth_CV.SteinDiv();
		
		
  //cov_mat_kth get_cov_seg(path, actionNames, scale_factor, shift, scene, segment_length);
  //get_cov_seg.calculate( all_people, dim );
  
  
  
  
  return 0;
  
}
