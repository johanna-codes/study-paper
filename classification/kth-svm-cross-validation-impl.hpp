#include "omp.h"

inline
kth_cv_svm::kth_cv_svm(const std::string in_path,
		       const std::string in_actionNames,  
		       const field<std::string> in_all_people,
		       const int in_scale_factor, 
		       const int in_shift,
		       const int in_scene, //only for kth
		       const int in_dim 
):path(in_path), actionNames(in_actionNames), all_people (in_all_people), scale_factor(in_scale_factor), shift(in_shift), total_scenes(in_scene), dim(in_dim)
{
  actions.load( actionNames );  
  
  
}


// Log-Euclidean Distance

inline
void
kth_cv_svm::logEucl()
{
 logEucl_distances();

 
}


inline
void
kth_cv_svm::logEucl_distances()
{
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  float acc;
  acc = 0;
  
  //int n_test = n_peo*n_actions*total_scenes - 1; // - person13_handclapping_d3
  int n_test = n_peo*n_actions*total_scenes; // - person13_handclapping_d3
  
  vec real_labels;
  vec est_labels;
  field<std::string> test_video_list(n_test);
  
  
  
  real_labels.zeros(n_test);
  est_labels.zeros(n_test);
  
  int k=0;
  int sc = 1; // = total scenes
  
  mat peo_act(n_test,2);
  
  for (int pe = 0; pe< n_peo; ++pe)
  {
    for (int act=0; act<n_actions; ++act)
    {
      peo_act (k,0) = pe;
      peo_act (k,1) = act;
      k++;
    }
  }
  
    std::stringstream load_sub_path;
    load_sub_path  << path << "cov_matrices/kth-one-cov-mat-dim" << dim << "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
    
  //omp_set_num_threads(8); //Use only 8 processors
  #pragma omp parallel for 
  for (int n = 0; n< n_test; ++n)
  {
    
    int pe  = peo_act (n,0);
    int act = peo_act (n,1);
    
 
    
    int tid=omp_get_thread_num();
    vec dist_video_i;
    
    #pragma omp critical
    cout<< "Processor " << tid <<" doing "<<  all_people (pe) << "_" << actions(act) << endl;
    
    std::stringstream load_cov;
    load_cov << load_sub_path.str() << "/LogMcov_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
    
    //#pragma omp critical
    //cout << load_cov_seg.str() << endl;
    
    dist_video_i = dist_logEucl_one_video( pe, load_sub_path.str(), load_cov.str());
    
    //save dist_video_i person, action  
    std::stringstream save_vec_dist;
    save_vec_dist << "./svm/logEucl/dist_vector_" << all_people (pe) << "_" << actions(act) << ".h5" ;
    
    
    #pragma omp critical
    dist_video_i.save(save_vec_dist.str(), hdf5_binary);

  }
  
}


inline
vec
kth_cv_svm::dist_logEucl_one_video(int pe_test, std::string load_sub_path, std::string load_cov)
{
  //wall_clock timer;
  //timer.tic();
  
  mat logMtest_cov;
  logMtest_cov.load(load_cov);
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  //double dist;
  double tmp_dist;
  vec dist;
  int num_dist = (n_peo-1)*n_actions;
  dist.zeros(num_dist);
  tmp_dist = datum::inf;
  
  
  double est_lab;
  int sc =1;
  int k=0;
  
  for (int pe_tr = 0; pe_tr< n_peo; ++pe_tr)
  {
    if (pe_tr!= pe_test)
    {	     
     
      //cout << " " << all_people (pe_tr);

	for (int act=0; act<n_actions; ++act)
	{
	  
	   std::stringstream load_cov_tr;
	   load_cov_tr << load_sub_path << "/LogMcov_" <<  all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".h5";
	   mat logMtrain_cov;
	   logMtrain_cov.load( load_cov_tr.str() );
	   tmp_dist = norm( logMtest_cov - logMtrain_cov, "fro");
	   dist(k) = tmp_dist;
	   ++k;
	}
    }
  }
  

  return dist;
  
}


