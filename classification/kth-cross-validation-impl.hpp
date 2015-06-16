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
 
   int acc;
   acc = 0;
   
   int n_test = n_peo*n_actions*total_sc;
   
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
	 vec count = zeros<vec>( n_actions );
	 
	 for (int s=0; s<num_s; ++s)
	 {
	   std::stringstream load_cov_seg;
	   load_cov_seg << load_sub_path.str() << "/LogMcov_seg" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
	   
	   cout << "LogMcov_seg" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5" << endl;
	   //debe devolver el est_labe de ese segmento
	   est_lab_segm(s) = one_seg_est_lab( pe, load_sub_path.str(),  load_cov_seg.str());
	   count( est_lab_segm(s) )++;
	   //getchar();
	 }
	 
	 uword  index_video;
	 double max_val = count.max(index_video);
	 //est_lab_segm.t().print("est_lab_segm");
	 cout << "This video is " << actions(act) << " and was classified as class: " << actions(index_video ) << endl;
	 
	 if (index_video == act)  {
	    acc++;
	    
	  }
      
	 getchar();
       }
       
       
     }
   }
   
   cout << "Performance: " << acc*100/n_test << " %" << endl;
   
 }
 
 
 inline
 uword
 kth_cv::one_seg_est_lab(int pe_test, std::string load_sub_path, std::string segm_name)
 {
   wall_clock timer;
   timer.tic();

   mat logMtest_cov;
   logMtest_cov.load(segm_name);

   int n_actions = actions.n_rows;
   int n_peo =  all_people.n_rows;
   
   double dist, tmp_dist;
   tmp_dist = datum::inf;
   
   
   double est_lab;
   cout << "Comparing with person ";
   
   for (int pe_tr = 0; pe_tr< n_peo; ++pe_tr)
   {
     if (pe_tr!= pe_test)
     {	     
       
       cout << " " << all_people (pe_tr);
       

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
	     load_cov_seg_tr << load_sub_path << "/LogMcov_seg" << s_tr << "_" << all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".h5";
	     
	     //cout << "Comparing with cov_seg" << s_tr << "_"<< all_people (pe_tr) << "_" << actions(act) << "_dim" << dim  << ".h5" << endl;
	     mat logMtrain_cov;
	     logMtrain_cov.load( load_cov_seg_tr.str() );
	     
	     //logMtest_cov.print("logMtest_cov");
	     //train_cov.print("train_cov");
	     
	     dist = norm( logMtest_cov - logMtrain_cov, "fro");

	     //cout << "dist " << dist << endl;
	     
	     if (dist < tmp_dist)
	     {
	       //cout << "Es menor" << endl;
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
   
   double n = timer.toc();
   //cout << "number of seconds: " << n << endl;
   cout << " est_lab "<< est_lab << endl << endl;
   //getchar();
   return est_lab;
   
 }