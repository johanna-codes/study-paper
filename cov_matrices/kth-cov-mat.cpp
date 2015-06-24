#include <omp.h>
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <armadillo>
#include <iomanip>
#include <vector>

using namespace std;
using namespace arma;

#include "cov-mat-def.hpp"
#include "cov-mat-impl.hpp"




//Home
//const std::string path = "/media/johanna/HD1T/codes/datasets_codes/KTH/"; 

//WANDA
const std::string path = "/home/johanna/codes/codes-git/study-paper/trunk/features";



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
  int total_scenes = 1;
  int segment_length = 20;
  int   dim = 14; //Action Recognition from Video Using feature Covariance Matrices
  
  field<string> all_people;
  all_people.load(peopleList);
  
  cov_mat_kth get_cov_seg(path, actionNames, scale_factor, shift, total_scenes, segment_length);
  get_cov_seg.calculate( all_people, dim );
  
  
  
  
  return 0;
  
}
