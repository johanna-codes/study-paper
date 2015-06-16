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
   total_sc = 1;
   
 }
 
 inline
 void
 kth_cv::logEucl()
 {
   
   int n_actions = actions.n_rows;
   int n_peo =  all_people.n_rows;
 
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
	 uvec  est_lab_segm;
	 est_lab_segm.zeros(num_s);
	 
	 for (int s=0; s<num_s; ++s)
	 {
	   std::stringstream load_cov_seg;
	   load_cov_seg << load_sub_path.str() << "/cov_seg" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
	   
	   cout << "cov_seg" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5" << endl;
	   //debe devolver el est_labe de ese segmento
	   est_lab_segm(s) = one_seg_est_lab( pe, load_sub_path.str(),  load_cov_seg.str());
	   //getchar();
	 }
       }
     }
   }
 }
 
 
 inline
 uword
 kth_cv::one_seg_est_lab(int pe_test, std::string load_sub_path, std::string segm_name)
 {
   
   ri_metrics Ri_met;
   mat test_cov;
   test_cov.load(segm_name);

   int n_actions = actions.n_rows;
   int n_peo =  all_people.n_rows;
   
   double dist, tmp_dist;
   tmp_dist = datum::inf;
   
   
   double est_lab;

   
   for (int pe_tr = 0; pe_tr< n_peo; ++pe_tr)
   {
     while (pe_tr!= pe_test)
     {
       for (int sc = 1; sc<=total_sc; ++sc) //scene
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
	     
	     cout << "Comparing with cov_seg" << s_tr << "_"<< all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".h5" << endl;
	     mat train_cov;
	     train_cov.load( load_cov_seg_tr.str() );
	     
	     //test_cov.print("test_cov");
	     //train_cov.print("train_cov");
	     
	     
	     dist = Ri_met.logEucl(test_cov, train_cov);
	     //cout << "dist " << dist << endl;
	     
	     if (dist < tmp_dist)
	     {
	       cout << "Es menor" << endl;
	       tmp_dist = dist;
	       est_lab = act;
	     }
	     //cout << "Press a key" << endl;
	     //getchar();
	       
	
	     
	     

	   }
	   
	 }

       }
     }
   }
   
   cout << "est_lab "<< est_lab << endl;
   getchar();
   return est_lab;
   
 }