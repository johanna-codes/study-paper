 inline
 kth_cv::kth_cv(const std::string in_path,
		const std::string in_actionNames,  
		const field<std::string> in_all_people,
		const int in_scale_factor, 
		const int in_shift,
		const int in_scene, //only for kth
		const int in_segment_length,
		const int in_dim 
):path(in_path), actionNames(in_actionNames), all_people (in_all_people), scale_factor(in_scale_factor), shift(in_shift), scene(in_scene), segment_length(in_segment_length), dim(in_dim)
 {
   actions.load( actionNames );  
   
 }
 
 inline
 void
 kth_cv::logEucl()
 {

   int n_actions = actions.n_rows;
   int n_peo =  all_people.n_rows;
   int total_sc = 1;
   vec acc;
   acc.zeros(n_peo*total_sc);
   
  for (int sc = 1; sc<=total_sc; ++sc) //scene
  {
    for (int pe = 0; pe< n_peo; ++pe)
    {
      for (int act=0; act<n_actions; ++act)
      {

	//load number of segments
	
	vec total_seg; 
	int num_s;
	std::stringstream load_sub_path;
	std::stringstream load_num_seg;
	
	
	
	load_sub_path  << path << "/kth-cov-mat/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
	load_num_seg << load_sub_path.str() << "/num_seg_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".dat";
	total_seg.load( load_num_seg.str());
	num_s = total_seg(0);
	
	for (int s=0; s<num_s; ++s)
	{
	
	  std::stringstream load_cov_seg;
	  load_cov_seg << load_sub_path.str() << "/cov_seg_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
	  mat borrame;
	  cout << load_cov_seg.str() << endl;	
	  borrame.load( load_cov_seg.str() );
	  getchar();
	  
	
	  
	  
	  
	   //debe devolver el est_labe de ese segmento
	   //one_seg_est_lab(int pe, string segm_name);
	   //getchar();
	}
	  
	 

      }
    }
  }
   
}

/*

inline
int
kth-cv::one_seg_est_lab(int pe_test, tring segm_name)
 {
   
   mat test_cov;
   test_cov.load(segm_name);
   
   
   all_people = in_all_people;
   dim = in_dim;
   int n_actions = actions.n_rows;
   int n_peo =  all_people.n_rows;

   // Crear nombre de testing
   
   double prev_dist=0;
   double lab = 0;
   
   for (int pe = 0; pe< n_peo; ++pe)
    {
      while (pe!= pe_test)
      {
	for (int sc = 1; sc<=1; ++sc) //scene
	{
	  for (int act=0; act<n_actions; ++act)
	  {
	    
	    //cargar train cov
	    
	     mat test_cov;
	     test_cov.load(train_segm_name);
	    
	  }



      }
    }
  }
   
 }*/