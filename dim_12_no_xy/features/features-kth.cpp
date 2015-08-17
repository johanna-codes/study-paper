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

#include "optflow-feat-def.hpp"
#include "optflow-feat-impl.hpp"




//Home
//const std::string path = "/media/johanna/HD1T/codes/datasets_codes/KTH/"; 

//WANDA
const std::string path = "/home/johanna/codes/datasets_codes/KTH/";



const std::string peopleList = "people_list.txt";
const std::string  actionNames = "actionNames.txt";

///kth
int ori_col = 160;
int ori_row = 120;





int
main(int argc, char** argv)
{
  
  vec vec_shift;
  vec_shift << -25 << -20 << -15 << -10 << -5 << 5 << 10 << 15 << 20 << 25 << endr;
  int scale_factor =1;
  
  int total_scene = 1; 
  int dim = 12; // Without x and y.
  
  field<string> all_people;
  all_people.load(peopleList);
  
  for (int i=0; i< vec_shift.n_elem; ++i)
  {
    
    int shift = vec_shift(i);
    cout << "Features for for shift " << shift << endl;
    opt_feat opt_feat_kth(path, actionNames, ori_col, ori_row, scale_factor, shift, total_scene, dim);
    opt_feat_kth.features_all_videos( all_people );
  }
  
  
  
  return 0;
  
}

// int
// main(int argc, char** argv)
// {
//   
//    if(argc < 3 )
//   {
//     cout << "usage: " << argv[0] << " scale_factor " << " shift_factor " << endl;
//     return -1;
//   }
//   
//   
//   int scale_factor = atoi( argv[1] );
//   int shift = atoi( argv[2] );
//   int total_scene = 1; 
//   int dim = 12; // Without x and y.
//   
//   field<string> all_people;
//   all_people.load(peopleList);
//   
//   opt_feat opt_feat_kth(path, actionNames, ori_col, ori_row, scale_factor, shift, total_scene, dim);
//   opt_feat_kth.features_all_videos( all_people );
//   
//   
//   
//   
//   return 0;
//   
// }
