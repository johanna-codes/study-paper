#include "omp.h"

inline
kth_cv_svm_LogEucl::kth_cv_svm_LogEucl(const std::string in_path,
		       const std::string in_actionNames,  
		       const field<std::string> in_all_people,
		       const int in_scene, //only for kth
		       const int in_dim 
):path(in_path), actionNames(in_actionNames), all_people (in_all_people), total_scenes(in_scene), dim(in_dim)
{
  actions.load( actionNames );  
  
  
}


// Log-Euclidean Distance

inline
void
kth_cv_svm_LogEucl::train( int tr_scale, int tr_shift )
{
  distances(tr_scale, tr_shift);
  svm_train(); //cross validation;
  
  
}

inline
void
kth_cv_svm_LogEucl::svm_train()
{
  
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  
  //int n_test = n_peo*n_actions*total_scenes - 1; // - person13_handclapping_d3
  int n_test = (n_peo-1)*n_actions*total_scenes; // - person13_handclapping_d3
  int n_dim = n_test;
  int sc = 1; // = total scenes
  fvec dist_vector;
  
  
  for (int pe_ts=0; pe_ts<n_peo; ++pe_ts)
  {
    
    fmat training_data;
    fvec lab;
    training_data.zeros(n_dim,n_test);
    lab.zeros(n_test);
    int k=0;
    for (int pe_tr=0; pe_tr<n_peo; ++pe_tr)
    {
      if(pe_tr!=pe_ts)
	
	for (int act=0; act<n_actions; act++)
	{

	    std::stringstream load_vec_dist;
	    load_vec_dist << path << "./classification/kth-svm/logEucl/dist_vector_" << all_people (pe_tr) << "_" << actions(act) << ".h5" ;
	    dist_vector.load( load_vec_dist.str() );
	    training_data.col(k) = dist_vector;
	    lab(k) = act;
	    ++k;
	}
    }
    
    //Training the model with OpenCV
    cout << "Using SVM to train run " << pe_ts+1 << endl;
    //cout << "Preparing data to train the data" << endl;
    cv::Mat cvMatTraining(n_test, n_dim, CV_32FC1);
    float fl_labels[n_test] ;
    
    
      for (uword m=0; m<n_test; ++m)
      {
	for (uword d=0; d<n_dim; ++d)
	{
	  cvMatTraining.at<float>(m,d) = training_data(d,m); 
	  //cout << " OpenCV: " << cvMatTraining.at<float>(m,d) << " - Arma: " <<training_data(d,m); 
	  
	}
	
	fl_labels[m] = lab(m);
	//cout <<" OpenCVLabel: " <<  fl_labels[m] << " ArmaLabel: " << labels(m) << endl;
	
      }
      
      cv::Mat cvMatLabels(n_test, 1, CV_32FC1,fl_labels );
      
      //cout << "Setting parameters" << endl;
      CvSVMParams params;
      params.svm_type    = CvSVM::C_SVC;
      params.kernel_type = CvSVM::LINEAR; 
      //params.gamma = 1;
      params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER,  (int)1e7, 1e-6);
      //params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);
      
      // Train the SVM
      //cout << "Training" << endl;
      CvSVM SVM;
      SVM.train( cvMatTraining , cvMatLabels, cv::Mat(), cv::Mat(), params);

      
      std::stringstream save_svm_model;
      save_svm_model << "./svm_models/logEucl_svm_run_" << pe_ts+1;
      SVM.save( save_svm_model.str().c_str() );
  }
}


inline
void
kth_cv_svm_LogEucl::test()
{
  
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  
  //int n_test = n_peo*n_actions*total_scenes - 1; // - person13_handclapping_d3
  int n_test = (n_peo-1)*n_actions*total_scenes; // - person13_handclapping_d3
  int n_dim = n_test;
  int sc = 1; // = total scenes
  fvec dist_vector;
  
  float acc=0;
  vec real_labels;
  vec est_labels;
  field<std::string> test_video_list(n_peo*n_actions);
  
  
  real_labels.zeros(n_peo*n_actions);
  est_labels.zeros(n_peo*n_actions);
  
  int j =0;
  
  std::stringstream load_sub_path;
  load_sub_path  << path << "cov_matrices/kth-one-cov-mat-dim" << dim << "/sc" << sc << "/scale" << ts_scale << "-shift"<< ts_shift ;

  
  
  
  for (int pe_ts=0; pe_ts<n_peo; ++pe_ts)
  {
    
    CvSVM SVM;

    std::stringstream load_svm_model;
    load_svm_model << "./svm_models/logEucl_svm_run_" << pe_ts+1;
    SVM.load( load_svm_model.str().c_str() );
    
    for (int act_ts =0; act_ts<n_actions; ++act_ts)
    {
	 vec test_dist;
	 
	 std::stringstream load_cov;
	 load_cov << load_sub_path.str() << "/LogMcov_" << all_people (pe_ts) << "_" << actions(act_ts) << "_dim" << dim  << ".h5";
	 vec test_dist;
	 test_dist = dist_one_video( pe_ts, load_sub_path.str(), load_cov.str());	
	 
	 
	 
	 cv::Mat cvMatTesting_onevideo(1, n_dim, CV_32FC1);
	 
	 for (uword d=0; d<n_dim; ++d)
	 {
	   cvMatTesting_onevideo.at<float>(0,d) = test_dist(d); 
	   
	}
   
	float response = SVM.predict(cvMatTesting_onevideo, true);
	
	//cout << "response " << response << endl;
	real_labels(j) = act_ts;
	est_labels(j) = response;
	test_video_list(j) = load_vec_dist.str();
	j++;
	
	if (response == act_ts)  {
	  acc++;
	  
	}
      
      
      }
      
      
      std::stringstream main_save;
      main_save << "./svm_results_2/LogEucl_scale" <<  ts_scale << "-shift"<< ts_shift;
      
      
      std::stringstream save1, save2,save3;
      
      save1 << main_save.str() << "_real_labels.dat";
      save2 << main_save.str() << "_est_labels.dat";
      save3 << main_save.str() << "_test_video_list.dat";
      
      real_labels.save(save1.str(), raw_ascii);
      est_labels.save( save2.str(), raw_ascii);
      test_video_list.save(save3.str(), raw_ascii); 
    
  }
  cout << "Performance for Log_Euclidean Distance: " << acc*100/(n_peo*n_actions) << " %" << endl;
}


///

inline
void
kth_cv_svm_LogEucl::distances(int scale_factor, int shift)
{
  
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  
  int scale2 = scale_factor;
  int shift2 = shift;
  
  
  //int n_test = n_peo*n_actions*total_scenes - 1; // - person13_handclapping_d3
  int n_test = n_peo*n_actions*total_scenes; // - person13_handclapping_d3
  
  
  
  
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
  load_sub_path  << path << "cov_matrices/kth-one-cov-mat-dim" << dim << "/sc" << sc << "/scale" << scale2 << "-shift"<< shift2;
  
  //omp_set_num_threads(8); //Use only 8 processors
  #pragma omp parallel for 
  for (int n = 0; n< n_test; ++n)
  {
    int pe  = peo_act (n,0);
    int act = peo_act (n,1);
    
    int tid=omp_get_thread_num();
    vec dist_video_i;
    
    //#pragma omp critical
    //cout<< "Processor " << tid <<" doing "<<  all_people (pe) << "_" << actions(act) << endl;
    
    std::stringstream load_cov;
    load_cov << load_sub_path.str() << "/LogMcov_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
    
    //#pragma omp critical
    //cout << load_cov_seg.str() << endl;
    
    dist_video_i = dist_one_video( pe, load_sub_path.str(), load_cov.str());
    //dist_video_i = dist_video_i/norm(dist_video_i,2);

    //save dist_video_i person, action  
    std::stringstream save_vec_dist;
    save_vec_dist << "./kth-svm/logEucl/dist_vector_" << all_people (pe) << "_" << actions(act) << ".h5" ;
    
    
    #pragma omp critical
    dist_video_i.save(save_vec_dist.str(), hdf5_binary);
    
  }
  
}


inline
vec
kth_cv_svm_LogEucl::dist_one_video(int pe_test, std::string load_sub_path, std::string load_cov)
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


