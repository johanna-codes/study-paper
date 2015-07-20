#include "omp.h"

inline
kth_cv_omp::kth_cv_omp(const std::string in_path,
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
kth_cv_omp::logEucl()
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
    uword est_label_video_i;
    
    //#pragma omp critical
    //cout<< "Processor " << tid <<" doing "<<  all_people (pe) << "_" << actions(act) << endl;
    
    std::stringstream load_cov;
    load_cov << load_sub_path.str() << "/LogMcov_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
    
    //#pragma omp critical
    //cout << load_cov_seg.str() << endl;
    
    est_label_video_i = logEucl_one_video( pe, load_sub_path.str(), load_cov.str());
    
    real_labels(n)=act;
    est_labels(n)=est_label_video_i;
 
    
    #pragma omp critical
    {
    if (est_label_video_i == act)
    {acc++;  }
    }
    
  }
  
  real_labels.save("./results_onesegment/Log_Eucl_real_labels.dat", raw_ascii);
  est_labels.save("./results_onesegment/Log_Eucl_est_labels.dat", raw_ascii);
  test_video_list.save("./results_onesegment/Log_Eucl_test_video_list.dat", raw_ascii);
  cout << "Performance: " << acc*100/n_test << " %" << endl;
  
}


inline
uword
kth_cv_omp::logEucl_one_video(int pe_test, std::string load_sub_path, std::string load_cov)
{
  //wall_clock timer;
  //timer.tic();
  
  mat logMtest_cov;
  logMtest_cov.load(load_cov);
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  double dist, tmp_dist;
  tmp_dist = datum::inf;
  
  
  double est_lab;
  
  for (int pe_tr = 0; pe_tr< n_peo; ++pe_tr)
  {
    if (pe_tr!= pe_test)
    {	     
      
      //cout << " " << all_people (pe_tr);
      
      
      for (int sc = 1; sc<=total_scenes; ++sc) //scene
      {
	for (int act=0; act<n_actions; ++act)
	{
	  
	   std::stringstream load_cov_tr;
	   load_cov_tr << load_sub_path << "/LogMcov_" <<  all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".h5";
	   mat logMtrain_cov;
	   logMtrain_cov.load( load_cov_tr.str() );
	   dist = norm( logMtest_cov - logMtrain_cov, "fro");
	    
	    if (dist < tmp_dist)
	    {
	      tmp_dist = dist;
	      est_lab = act;
	    }
	  
	}
      }
    }
  }
  

  return est_lab;
  
}




// Stein Divergence 

inline
void
kth_cv_omp::SteinDiv()
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
    uword est_label_video_i;
    
    //#pragma omp critical
    //cout<< "Processor " << tid <<" doing "<<  all_people (pe) << "_" << actions(act) << endl;
    
    std::stringstream load_cov;
    load_cov << load_sub_path.str() << "/cov_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
    
    //#pragma omp critical
    //cout << load_cov_seg.str() << endl;
    
    est_label_video_i = SteinDiv_one_video( pe, load_sub_path.str(), load_cov.str());
    
    real_labels(n)=act;
    est_labels(n)=est_label_video_i;
 
    
    #pragma omp critical
    {
    if (est_label_video_i == act)
    {acc++;  }
    }
    
  }
  
  real_labels.save("./results_onesegment/Stein_div_real_labels.dat", raw_ascii);
  est_labels.save("./results_onesegment/Stein_div_est_labels.dat", raw_ascii);
  test_video_list.save("./results_onesegment/Stein_div_test_video_list.dat", raw_ascii);
  cout << "Performance for SteinDiv " << acc*100/n_test << " %" << endl;

  
}


inline
uword
kth_cv_omp::SteinDiv_one_video(int pe_test, std::string load_sub_path, std::string load_cov)
{
  //wall_clock timer;
  //timer.tic();
  
  mat test_cov;
  test_cov.load(load_cov);
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  double dist_stein, tmp_dist;
  tmp_dist = datum::inf;
  
  
  double est_lab;
  
  for (int pe_tr = 0; pe_tr< n_peo; ++pe_tr)
  {
    if (pe_tr!= pe_test)
    {	     

      for (int sc = 1; sc<=total_scenes; ++sc) //scene
      {
	for (int act = 0; act<n_actions; ++act)
	{
	  
	   std::stringstream load_cov_tr;
	   load_cov_tr << load_sub_path << "/cov_" <<  all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".h5";
	   mat train_cov;
	   train_cov.load( load_cov_tr.str() );
	   
	     //Esto vienen de PartI4. No recuerdo mas :(
	    double det_op1 = det( diagmat( (test_cov + train_cov)/2 ) );
	    double det_op2 = det( diagmat( ( test_cov%train_cov ) ) );
	    dist_stein =  log( det_op1 ) - 0.5*log( det_op2 ) ;
	    
	    
	    if (dist_stein < tmp_dist)
	    {
	      tmp_dist = dist_stein;
	      est_lab = act;
	    }
	  
	}
      }
    }
  }
  

  return est_lab;
  
}






///Grassmann Manifolds

///Projection Metric
inline
float
kth_cv_omp::proj_grass(int in_p)
{
  
  p = in_p;
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
  load_sub_path  << path << "grass_points/kth-grass-point-one-dim" << dim << "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
    
    
  //omp_set_num_threads(8); //Use only 8 processors
  #pragma omp parallel for 
  for (int n = 0; n< n_test; ++n)
  {
    
    int pe  = peo_act (n,0);
    int act = peo_act (n,1);
    
 
    
    int tid=omp_get_thread_num();
    uword est_label_video_i;
    
    //#pragma omp critical
    //cout<< "Processor " << tid <<" doing "<<  all_people (pe) << "_" << actions(act) << endl;
    
    std::stringstream load_Gnp;
    load_Gnp << load_sub_path.str() << "/grass_pt_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << "_p" << p<< ".h5";

    
    //#pragma omp critical
    //cout << load_cov_seg.str() << endl;
    
    est_label_video_i = ProjectionMetric_one_video( pe, load_sub_path.str(), load_Gnp.str(), p);
    
    real_labels(n)=act;
    est_labels(n)=est_label_video_i;
 
    
    #pragma omp critical
    {
    if (est_label_video_i == act)
    {acc++;  }
    }
    
  }
  
//    real_labels.save("./results_onesegment/grass_PM_real_labels.dat", raw_ascii);
//    est_labels.save("./results_onesegment/grass_PM_est_labels.dat", raw_ascii);
//    test_video_list.save("./results_onesegment/grass_PM_test_video_list.dat", raw_ascii);
//    cout << "Performance for Projection Metric: " << acc*100/n_test << " %" << endl;
//    acc =   acc*100/n_test;
   return acc;
}


inline
uword
kth_cv_omp::ProjectionMetric_one_video(int pe_test, std::string load_sub_path, std::string load_Gnp, int p)
{
  //wall_clock timer;
  //timer.tic();
  
  grass_metric grass_dist;
  mat grass_point_test;
  grass_point_test.load(load_Gnp);
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  double dist, tmp_dist;
  tmp_dist = datum::inf;
  
  
  double est_lab;
  
  for (int pe_tr = 0; pe_tr< n_peo; ++pe_tr)
  {
    if (pe_tr!= pe_test)
    {	     
      
      //cout << " " << all_people (pe_tr);
      
      
      for (int sc = 1; sc<=total_scenes; ++sc) //scene
      {
	for (int act=0; act<n_actions; ++act)
	{
	  
	   std::stringstream load_Gnp_tr;
	   load_Gnp_tr << load_sub_path << "/grass_pt_" <<  all_people (pe_tr) << "_" << actions(act) << "_dim" << dim << "_p" << p << ".h5";
	   

	   mat grass_point_train;
	   grass_point_train.load( load_Gnp_tr.str() );
	   
	   dist = grass_dist.proj_metric(grass_point_test,grass_point_train, p);

	    
	    if (dist < tmp_dist)
	    {
	      tmp_dist = dist;
	      est_lab = act;
	    }
	  
	}
      }
    }
  }

  return est_lab;
  
}

///Binet-Cauchy Metric
inline
float
kth_cv_omp::BC_grass(int in_p)
{
  p = in_p;
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
  load_sub_path  << path << "grass_points/kth-grass-point-one-dim" << dim << "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
    
    
  //omp_set_num_threads(8); //Use only 8 processors
  #pragma omp parallel for 
  for (int n = 0; n< n_test; ++n)
  {
    
    int pe  = peo_act (n,0);
    int act = peo_act (n,1);
    
 
    
    int tid=omp_get_thread_num();
    uword est_label_video_i;
    
    //#pragma omp critical
    //cout<< "Processor " << tid <<" doing "<<  all_people (pe) << "_" << actions(act) << endl;
    
    std::stringstream load_Gnp;
    load_Gnp << load_sub_path.str() << "/grass_pt_" << all_people (pe) << "_" << actions(act) << "_dim" << dim << "_p" << p  << ".h5";

    
    //#pragma omp critical
    //cout << load_cov_seg.str() << endl;
    
    est_label_video_i = BinetCauchyMetric_one_video( pe, load_sub_path.str(), load_Gnp.str());
    
    real_labels(n)=act;
    est_labels(n)=est_label_video_i;
    
 
    
    #pragma omp critical
    {
    if (est_label_video_i == act)
    {acc++;  }
    }
    
  }
  
//    real_labels.save("./results_onesegment/grass_BC_real_labels.dat", raw_ascii);
//    est_labels.save("./results_onesegment/grass_BC_est_labels.dat", raw_ascii);
//    test_video_list.save("./results_onesegment/grass_BC_test_video_list.dat", raw_ascii);
//    cout << "Performance for Binet-Cauchy: " << acc*100/n_test << " %" << endl;  
//    acc =   acc*100/n_test;
   return acc;
  
}


inline
uword
kth_cv_omp::BinetCauchyMetric_one_video(int pe_test, std::string load_sub_path, std::string load_Gnp)
{
  //wall_clock timer;
  //timer.tic();
  
  grass_metric grass_dist;
  mat grass_point_test;
  grass_point_test.load(load_Gnp);
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  double dist, tmp_dist;
  tmp_dist = datum::inf;
  
  
  double est_lab;
  
  for (int pe_tr = 0; pe_tr< n_peo; ++pe_tr)
  {
    if (pe_tr!= pe_test)
    {	     
      
      //cout << " " << all_people (pe_tr);
      
      
      for (int sc = 1; sc<=total_scenes; ++sc) //scene
      {
	for (int act=0; act<n_actions; ++act)
	{
	  
	   std::stringstream load_Gnp_tr;
	   load_Gnp_tr << load_sub_path << "/grass_pt_" <<  all_people (pe_tr) << "_" << actions(act) << "_dim" << dim << "_p" << p << ".h5";
	   

	   mat grass_point_train;
	   grass_point_train.load( load_Gnp_tr.str() );
	   
	   
	   dist = grass_dist.BC_metric(grass_point_test,grass_point_train);

	    
	    if (dist < tmp_dist)
	    {
	      tmp_dist = dist;
	      est_lab = act;
	    }
	  
	}
      }
    }
  }

  return est_lab;
  
}

