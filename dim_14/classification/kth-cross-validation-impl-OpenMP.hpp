#include "omp.h"

inline
kth_cv_omp::kth_cv_omp(const std::string in_path,
		       const std::string in_actionNames,  
		       const field<std::string> in_all_people,
		       const int in_scale_factor, 
		       const int in_shift,
		       const int in_scene, //only for kth
		       const int in_segment_length,
		       const int in_dim 
):path(in_path), actionNames(in_actionNames), all_people (in_all_people), scale_factor(in_scale_factor), shift(in_shift), total_scenes(in_scene), segment_length(in_segment_length), dim(in_dim)
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
  
  //omp_set_num_threads(8); //Use only 8 processors
  //#pragma omp parallel for 
  for (int n = 0; n< n_test; ++n)
  {
    
    int pe  = peo_act (n,0);
    int act = peo_act (n,1);
    
    //load number of segments
    vec total_seg; 
    int num_s;
    std::stringstream load_sub_path;
    std::stringstream load_num_seg;
    
    load_sub_path  << path << "cov_matrices/kth-cov-mat_dim" << dim << "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
    load_num_seg << load_sub_path.str() << "/num_seg_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".dat";
    
    int tid=omp_get_thread_num();
    
    //#pragma omp critical
    cout<< "Processor " << tid <<" doing "<<  all_people (pe) << "_" << actions(act) << endl;
    
    total_seg.load( load_num_seg.str());
    num_s = total_seg(0);
    uvec  est_lab_segm;
    est_lab_segm.zeros(num_s);
    vec count = zeros<vec>( n_actions );
    
    //wall_clock timer;
    //timer.tic();
    
    for (int s=0; s<num_s; ++s)
    {
      std::stringstream load_cov_seg;
      load_cov_seg << load_sub_path.str() << "/LogMcov_seg" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
      //cout << "LogMcov_seg" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5" << endl;
      //debe devolver el est_labe de ese segmento
      est_lab_segm(s) = logEucl_one_seg_est_lab( pe, load_sub_path.str(),  load_cov_seg.str());
      //#pragma omp critical
      count( est_lab_segm(s) )++;
      
    }
    
    uword  index_video;
    double max_val = count.max(index_video);
    //est_lab_segm.t().print("est_lab_segm");
    cout << "This video is " << actions(act) << " and was classified as class: " << actions(index_video ) << endl;
    real_labels(n) = act;
    est_labels(n) = index_video;
    test_video_list(n) = load_sub_path.str();
    
    //#pragma omp critical
    if (index_video == act)
    {acc++;  }
    
  }
  
  real_labels.save("Log_Eucl_real_labels.dat", raw_ascii);
  est_labels.save("Log_Eucl_est_labels.dat", raw_ascii);
  test_video_list.save("Log_Eucl_test_video_list.dat", raw_ascii);
  cout << "Performance: " << acc*100/n_test << " %" << endl;
  
}


inline
uword
kth_cv_omp::logEucl_one_seg_est_lab(int pe_test, std::string load_sub_path, std::string segm_name)
{
  //wall_clock timer;
  //timer.tic();
  
  mat logMtest_cov;
  logMtest_cov.load(segm_name);
  
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
	  vec total_seg; 
	  int num_s;
	  std::stringstream load_num_seg;
	  load_num_seg << load_sub_path << "/num_seg_"<< all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".dat";
	  total_seg.load( load_num_seg.str());
	  num_s = total_seg(0);
	  
	  for (int s_tr=0; s_tr<num_s; ++s_tr)
	  {
	    std::stringstream load_cov_seg_tr;
	    load_cov_seg_tr << load_sub_path << "/LogMcov_seg" << s_tr << "_" << all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".h5";
	    
	    //cout << "Comparing with cov_seg" << s_tr << "_"<< all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".h5" << endl;
	    mat logMtrain_cov;
	    logMtrain_cov.load( load_cov_seg_tr.str() );
	    
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
  }
  
  //double n = timer.toc();
  //cout << "number of seconds: " << n << endl;
  //cout << " est_lab "<< est_lab << endl << endl;
  //getchar();
  return est_lab;
  
}



//DO IT
// Stein Divergence 

inline
void
kth_cv_omp::SteinDiv()
{
  cout << "SteinDiv"  << endl;
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  float acc;
  acc = 0;
  
  //int n_test = n_peo*n_actions*total_scenes - 1; // - person13_handclapping_d3
  int n_test = n_peo*n_actions*total_scenes; 
  
  int sc = total_scenes; //Doing only for scenario 1
  vec real_labels;
  vec est_labels;
  field<std::string> test_video_list(n_test);
  
  
  real_labels.zeros(n_test);
  est_labels.zeros(n_test);
  
  int k=0;
  
  
  for (int pe = 0; pe< n_peo; ++pe)
  {
    for (int act=0; act<n_actions; ++act)
    {
      vec total_seg; 
      int num_s;
      std::stringstream load_sub_path;
      std::stringstream load_num_seg;
      load_sub_path  << path << "/kth-cov-mat_dim" << dim << "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
      load_num_seg << load_sub_path.str() << "/num_seg_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".dat";
      total_seg.load( load_num_seg.str());
      num_s = total_seg(0);
      uvec  est_lab_segm;
      est_lab_segm.zeros(num_s);
      vec count = zeros<vec>( n_actions );
      
      for (int s=0; s<num_s; ++s)
      {
	std::stringstream load_cov_seg;
	load_cov_seg << load_sub_path.str() << "/cov_seg" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
	
	est_lab_segm(s) = SteinDiv_one_seg_est_lab( pe, load_sub_path.str(),  load_cov_seg.str());
	count( est_lab_segm(s) )++;
	//getchar();
      }
      
      uword  index_video;
      double max_val = count.max(index_video);
      //est_lab_segm.t().print("est_lab_segm");
      cout << "This video is " << actions(act) << " and was classified as class: " << actions(index_video ) << endl;
      
      
      real_labels(k) = act;
      est_labels(k) = index_video;
      test_video_list(k) = load_sub_path.str();
      
      real_labels.save("Stein_div_real_labels.dat", raw_ascii);
      est_labels.save("Stein_div_est_labels.dat", raw_ascii);
      test_video_list.save("Stein_div_test_video_list.dat", raw_ascii);
      k++;
      
      
      if (index_video == act)  {
	acc++;
	
      }
      
      
      
    }
  }
  
  cout << "Performance for SteinDiv" << acc*100/n_test << " %" << endl;
  
}


//DO IT!!!!!!!!!
inline
uword
kth_cv_omp::SteinDiv_one_seg_est_lab(int pe_test, std::string load_sub_path, std::string segm_name)
{
  //wall_clock timer;
  //timer.tic();
  
  mat test_cov;
  test_cov.load(segm_name);
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  double dist_stein, tmp_dist;
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
	  vec total_seg; 
	  int num_s;
	  std::stringstream load_num_seg;
	  load_num_seg << load_sub_path << "/num_seg_"<< all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".dat";
	  total_seg.load( load_num_seg.str());
	  num_s = total_seg(0);
	  
	  for (int s_tr=0; s_tr<num_s; ++s_tr)
	  {
	    std::stringstream load_cov_seg_tr;
	    load_cov_seg_tr << load_sub_path << "/cov_seg" << s_tr << "_" << all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".h5";
	    
	    mat train_cov;
	    train_cov.load( load_cov_seg_tr.str() );
	    
	    //Esto vienen de PartI4. No recuerdo mas :(
	    double det_op1 = det( diagmat( (test_cov + train_cov)/2 ) );
	    double det_op2 = det( diagmat( ( test_cov%train_cov ) ) );
	    dist_stein =  log( det_op1 ) - 0.5*log( det_op2 ) ;
	    
	    
	    
	    
	    //dist_stein = norm( logMtest_cov - logMtrain_cov, "fro");
	    
	    //cout << "dist " << dist << endl;
	    
	    if (dist_stein < tmp_dist)
	    {
	      //cout << "Es menor" << endl;
	      tmp_dist = dist_stein;
	      est_lab = act;
	    }
	    //cout << "Press a key" << endl;
	    //getchar();
	  }
	}
      }
    }
  }
  
  //double n = timer.toc();
  //cout << "number of seconds: " << n << endl;
  //cout << " est_lab "<< est_lab << endl << endl;
  //getchar();
  return est_lab;
  
}




///Grassmann Manifolds

///Projection Metric
inline
void
kth_cv_omp::proj_grass(int p)
{
  
   int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  
  float acc;
  acc = 0;
  
  //int n_test = n_peo*n_actions*total_scenes - 1; // - person13_handclapping_d3
  int n_test = n_peo*n_actions*total_scenes; 
  
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
  
  //omp_set_num_threads(8); //Use only 8 processors
  //#pragma omp parallel for 
  for (int n = 0; n< n_test; ++n)
  {
    
    int pe  = peo_act (n,0);
    int act = peo_act (n,1);
    
    //load number of segments
    vec total_seg; 
    int num_s;
    std::stringstream load_sub_path;
    std::stringstream load_num_seg;
    load_sub_path  << path << "grass_points/kth-grass-point-dim" << dim << "-L" << segment_length << "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift;
    load_num_seg << load_sub_path.str() << "/num_seg_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".dat";
    
    int tid=omp_get_thread_num();
    
    //#pragma omp critical
    cout<< "Processor " << tid <<" doing "<<  all_people (pe) << "_" << actions(act) << endl;
    
    total_seg.load( load_num_seg.str());
    num_s = total_seg(0);
    uvec  est_lab_segm;
    est_lab_segm.zeros(num_s);
    vec count = zeros<vec>( n_actions );
    
    //wall_clock timer;
    //timer.tic();
    
    for (int s=0; s<num_s; ++s)
    {
      std::stringstream load_gp_seg;
      load_gp_seg << load_sub_path.str() << "/grass_pt" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
      est_lab_segm(s) = ProjectionMetric_one_seg_est_lab( pe, load_sub_path.str(),  load_gp_seg.str(), p);
      //#pragma omp critical
      count( est_lab_segm(s) )++;
      
    }
    
    uword  index_video;
    double max_val = count.max(index_video);
    //est_lab_segm.t().print("est_lab_segm");
    cout << "This video is " << actions(act) << " and was classified as class: " << actions(index_video ) << endl;
    real_labels(n) = act;
    est_labels(n) = index_video;
    test_video_list(n) = load_sub_path.str();
    
    //#pragma omp critical
    if (index_video == act)
    {acc++;  }
    
  }
  
  real_labels.save("grass_PM_real_labels.dat", raw_ascii);
  est_labels.save("grass_PM_est_labels.dat", raw_ascii);
  test_video_list.save("grass_PM_test_video_list.dat", raw_ascii);
  cout << "Performance: " << acc*100/n_test << " %" << endl;
  
}


inline
uword
kth_cv_omp::ProjectionMetric_one_seg_est_lab(int pe_test, std::string load_sub_path, std::string segm_name, int p)
{
  //wall_clock timer;
  //timer.tic();
  
  grass_metric grass_dist;
  
  mat grass_point_test;
  grass_point_test.load(segm_name);
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  double dist, tmp_dist;
  tmp_dist = datum::inf;
  
  
  double est_lab;
  
  for (int pe_tr = 0; pe_tr< n_peo; ++pe_tr)
  {
    if (pe_tr!= pe_test)
    {	     

      for (int sc = 1; sc<=total_scenes; ++sc) //scene
      {
	for (int act=0; act<n_actions; ++act)
	{
	  vec total_seg; 
	  int num_s;
	  std::stringstream load_num_seg;
	  load_num_seg << load_sub_path << "/num_seg_"<< all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".dat";
	  total_seg.load( load_num_seg.str());
	  num_s = total_seg(0);
	  
	  for (int s_tr=0; s_tr<num_s; ++s_tr)
	  {
	    std::stringstream load_gp_seg_tr;
	    load_gp_seg_tr << load_sub_path << "/grass_pt" << s_tr << "_" << all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".h5";
	    
	    mat grass_point_train;
	    grass_point_train.load( load_gp_seg_tr.str() );
	    
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
  }
  
  //double n = timer.toc();
  //cout << "number of seconds: " << n << endl;
  //cout << " est_lab "<< est_lab << endl << endl;
  //getchar();
  return est_lab;
  
}

///Binet-Cauchy Metric

inline
void
kth_cv_omp::BC_grass()
{
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  
  float acc;
  acc = 0;
  
  //int n_test = n_peo*n_actions*total_scenes - 1; // - person13_handclapping_d3
  int n_test = n_peo*n_actions*total_scenes;
  
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
  
  //omp_set_num_threads(8); //Use only 8 processors
  //#pragma omp parallel for 
  for (int n = 0; n< n_test; ++n)
  {
    
    int pe  = peo_act (n,0);
    int act = peo_act (n,1);
    
    //load number of segments
    vec total_seg; 
    int num_s;
    std::stringstream load_sub_path;
    std::stringstream load_num_seg;
    load_sub_path  << path << "grass_points/kth-grass-point-dim" << dim << "-L" << segment_length << "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift;
    load_num_seg << load_sub_path.str() << "/num_seg_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".dat";
    
    int tid=omp_get_thread_num();
    
    //#pragma omp critical
    cout<< "Processor " << tid <<" doing "<<  all_people (pe) << "_" << actions(act) << endl;
    
    total_seg.load( load_num_seg.str());
    num_s = total_seg(0);
    uvec  est_lab_segm;
    est_lab_segm.zeros(num_s);
    vec count = zeros<vec>( n_actions );
    
    //wall_clock timer;
    //timer.tic();
    
    for (int s=0; s<num_s; ++s)
    {
      std::stringstream load_gp_seg;
      load_gp_seg << load_sub_path.str() << "/grass_pt" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
      //cout << "LogMcov_seg" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5" << endl;
      //debe devolver el est_labe de ese segmento
      est_lab_segm(s) = BinetCauchyMetric_one_seg_est_lab( pe, load_sub_path.str(),  load_gp_seg.str());
      //#pragma omp critical
      count( est_lab_segm(s) )++;
      
    }
    
    uword  index_video;
    double max_val = count.max(index_video);
    //est_lab_segm.t().print("est_lab_segm");
    cout << "This video is " << actions(act) << " and was classified as class: " << actions(index_video ) << endl;
    real_labels(n) = act;
    est_labels(n) = index_video;
    test_video_list(n) = load_sub_path.str();
    
    //#pragma omp critical
    if (index_video == act)
    {acc++;  }
    
  }
  
  real_labels.save("grass_BC_real_labels.dat", raw_ascii);
  est_labels.save("grass_BC_est_labels.dat", raw_ascii);
  test_video_list.save("grass_BC_test_video_list.dat", raw_ascii);
  cout << "Performance: " << acc*100/n_test << " %" << endl;
  
}


inline
uword
kth_cv_omp::BinetCauchyMetric_one_seg_est_lab(int pe_test, std::string load_sub_path, std::string segm_name)
{
  //wall_clock timer;
  //timer.tic();
  
  grass_metric grass_dist;
  
  mat grass_point_test;
  grass_point_test.load(segm_name);
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  double dist, tmp_dist;
  tmp_dist = datum::inf;
  
  
  double est_lab;
  
  for (int pe_tr = 0; pe_tr< n_peo; ++pe_tr)
  {
    if (pe_tr!= pe_test)
    {	     
      
      for (int sc = 1; sc<=total_scenes; ++sc) //scene
      {
	for (int act=0; act<n_actions; ++act)
	{
	  vec total_seg; 
	  int num_s;
	  std::stringstream load_num_seg;
	  load_num_seg << load_sub_path << "/num_seg_"<< all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".dat";
	  total_seg.load( load_num_seg.str());
	  num_s = total_seg(0);
	  
	  for (int s_tr=0; s_tr<num_s; ++s_tr)
	  {
	    std::stringstream load_gp_seg_tr;
	    load_gp_seg_tr << load_sub_path << "/grass_pt" << s_tr << "_" << all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".h5";
	    
	    mat grass_point_train;
	    grass_point_train.load( load_gp_seg_tr.str() );
	   
	    dist = grass_dist.BC_metric(grass_point_test,grass_point_train);
	    
	    if (dist < tmp_dist)
	    {
	      //cout << "Es menor" << endl;
	      tmp_dist = dist;
	      est_lab = act;
	    }
	  }
	}
      }
    }
  }
  return est_lab;
  
}



