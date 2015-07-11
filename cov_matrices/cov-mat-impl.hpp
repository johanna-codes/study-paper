inline
cov_mat_kth::cov_mat_kth( const std::string in_path,
			  const std::string in_actionNames,  
			  const int in_scale_factor, 
			  const int in_shift,
			  const int in_scene, //only for kth
			  const int in_segment_length
)
:path(in_path), actionNames(in_actionNames), scale_factor(in_scale_factor), shift(in_shift), total_scenes(in_scene), segment_length(in_segment_length)
{
  actions.load( actionNames );  
}


inline
void
cov_mat_kth::calculate( field<string> in_all_people, int  in_dim  )
{
  all_people = in_all_people;
  dim = in_dim;
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  //all_people.print("people");
  
  
  field <std::string> parallel_names(n_peo*n_actions,4); 
  int sc = total_scenes; //Solo estoy usando 1 
  int k =0;
  
  
      for (int pe = 0; pe< n_peo; ++pe)
      {
	for (int act=0; act<n_actions; ++act)
	{
	
	
	std::stringstream load_folder;
	std::stringstream load_feat_video_i;
	std::stringstream load_labels_video_i;
	
	
	load_folder << path <<"kth-features_dim" << dim <<  "_openMP/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;

	load_feat_video_i << load_folder.str() << "/" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
	load_labels_video_i << load_folder.str() << "/lab_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
	
	
	std::ostringstream ss1;
	std::ostringstream ss2;
	ss1 << pe;
	ss2 << act;

	
	parallel_names(k,0) = load_feat_video_i.str();
	parallel_names(k,1) = load_labels_video_i.str();
	parallel_names(k,2) = ss1.str();
	parallel_names(k,3) = ss2.str();
	k++;
	
      }
    }
    
   
   //Aca podria hacer el paparelo

   #pragma omp parallel for 
    for (int k = 0; k< parallel_names.n_rows; ++k)
    {
      std::string load_feat_video_i   = parallel_names(k,0);
      std::string load_labels_video_i = parallel_names(k,1);
      
      int pe   = atoi( parallel_names(k,2).c_str() );
      int act  = atoi( parallel_names(k,3).c_str() );
      
      cov_mat_kth::one_video(load_feat_video_i, load_labels_video_i, sc, pe, act );
    }

}


inline
void
cov_mat_kth::one_video( std::string load_feat_video_i,	std::string load_labels_video_i, int sc, int pe, int act )
{
  //#pragma omp critical
  //cout << load_feat_video_i << endl;
  
  mat mat_features_video_i;
  vec lab_video_i;
  
  mat_features_video_i.load( load_feat_video_i, hdf5_binary );
  lab_video_i.load( load_labels_video_i, hdf5_binary );
  int n_vec = lab_video_i.n_elem;
  int last = lab_video_i( n_vec - 1 );
  //cout << last << endl;
  
  int s = 0;
  
  std::stringstream save_folder;
  save_folder << "./kth-cov-mat_dim" << dim << "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
  
  
  for (int l=2; l<last-segment_length; l = l+4 )
  {
    running_stat_vec<rowvec> stat_seg(true);
    //int k =0;
    
    //cout << " " << l;
    
    
    for (int j=l; j< l + segment_length; ++j)
    {
      //k++;
      //cout << " " << j;
      uvec indices = find(lab_video_i == j);
      mat tmp_feat = mat_features_video_i.cols(indices);
      //cout << "row&col " << tmp_feat.n_rows << " & " << tmp_feat.n_cols << endl;
      for (int v=0; v < tmp_feat.n_cols; ++v)
      {
	vec sample = tmp_feat.col(v);
	stat_seg (sample);
	
      }
      
      
    }
    
    //cout << endl;
    //cout << " " << stat_seg.count();
    
    if (stat_seg.count()>100) // Cuando en el segmento hay mas de 100 vectores 
    {
    std::stringstream save_cov_seg;
    save_cov_seg << save_folder.str() << "/cov_seg" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
    
    std::stringstream save_LogMcov_seg;
    save_LogMcov_seg << save_folder.str() << "/LogMcov_seg" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
      
    double THRESH = 0.000001;
    mat cov_seg_i = stat_seg.cov();
    
    //Following Mehrtash suggestions as per email dated June26th 2014
    cov_seg_i = 0.5*(cov_seg_i + cov_seg_i.t());
    vec D;
    mat V;
    eig_sym(D, V, cov_seg_i);
    uvec q1 = find(D < THRESH);
    
    if (q1.n_elem>0)
    {
      for (uword pos = 0; pos < q1.n_elem; ++pos)
      {
	D( q1(pos) ) = THRESH;
	
      }
      
      cov_seg_i = V*diagmat(D)*V.t();  
      
    }  
    //end suggestion

     eig_sym(D, V, cov_seg_i);
     mat log_M = V*diagmat( log(D) )*V.t();
     #pragma omp critical
     {
     cov_seg_i.save( save_cov_seg.str(), hdf5_binary ); 
     log_M.save( save_LogMcov_seg.str(), hdf5_binary );
     }
     s++;
      
    }
    
    else {
     //cout << " " << stat_seg.count();
     //getchar();
     
    }
  }

  std::stringstream save_seg;
  vec total_seg; 
  total_seg.zeros(1);
  total_seg( 0 ) = s;
  save_seg << save_folder.str() << "/num_seg_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".dat";
  #pragma omp critical
  {
  total_seg.save( save_seg.str(), raw_ascii );
  cout <<  all_people (pe) << "_" << actions(act) << ". Total # of segments " << s << endl;
  }
  //cout << "press a key " ;
  //getchar();
}

///****************************************************************************************
///****************************************************************************************
//One covariance matrix per video

inline
void
cov_mat_kth::calculate_one_per_video( field<string> in_all_people, int  in_dim  )
{
  all_people = in_all_people;
  dim = in_dim;
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  //all_people.print("people");
  
  
  field <std::string> parallel_names(n_peo*n_actions,4); 
  int sc = total_scenes; //Solo estoy usando 1 
  int k =0;
  
  
      for (int pe = 0; pe< n_peo; ++pe)
      {
	for (int act=0; act<n_actions; ++act)
	{
	
	
	std::stringstream load_folder;
	std::stringstream load_feat_video_i;
	std::stringstream load_labels_video_i;
	
	
	//load_folder << path <<"kth-features_dim" << dim <<  "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
	load_folder << path <<"kth-features_dim" << dim <<  "_openMP/sc" <<  sc << "/scale" << scale_factor << "-shift"<< shift ;

	load_feat_video_i << load_folder.str() << "/" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
	load_labels_video_i << load_folder.str() << "/lab_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
	
	
	std::ostringstream ss1;
	std::ostringstream ss2;
	ss1 << pe;
	ss2 << act;

	
	parallel_names(k,0) = load_feat_video_i.str();
	parallel_names(k,1) = load_labels_video_i.str();
	parallel_names(k,2) = ss1.str();
	parallel_names(k,3) = ss2.str();
	k++;
	
      }
    }
    
   

   #pragma omp parallel for 
    for (int k = 0; k< parallel_names.n_rows; ++k)
    {
      std::string load_feat_video_i   = parallel_names(k,0);
      std::string load_labels_video_i = parallel_names(k,1);
      
      int pe   = atoi( parallel_names(k,2).c_str() );
      int act  = atoi( parallel_names(k,3).c_str() );
      
      cov_mat_kth::one_video_one_cov(load_feat_video_i, load_labels_video_i, sc, pe, act );
    }

}


inline
void
cov_mat_kth::one_video_one_cov( std::string load_feat_video_i, std::string load_labels_video_i, int sc, int pe, int act )
{
  //#pragma omp critical
  //cout << load_feat_video_i << endl;
  
  mat mat_features_video_i;
  
  mat_features_video_i.load( load_feat_video_i, hdf5_binary );

  int n_vec = mat_features_video_i.n_cols;

  
  std::stringstream save_folder;
  save_folder << "./kth-one-cov-mat-dim" << dim << "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
  
  running_stat_vec<rowvec> stat_seg(true);
  
  for (int l=0; l<n_vec; ++l)
  {
    
    vec sample = mat_features_video_i.col(l);
    stat_seg (sample);
    
  }


    std::stringstream save_cov_seg;
    save_cov_seg << save_folder.str() << "/cov_" <<  all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
    
    std::stringstream save_LogMcov_seg;
    save_LogMcov_seg << save_folder.str() << "/LogMcov_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
      
    double THRESH = 0.000001;
    mat cov_seg_i = stat_seg.cov();
    
    //Following Mehrtash suggestions as per email dated June26th 2014
    cov_seg_i = 0.5*(cov_seg_i + cov_seg_i.t());
    vec D;
    mat V;
    eig_sym(D, V, cov_seg_i);
    uvec q1 = find(D < THRESH);
    
    if (q1.n_elem>0)
    {
      for (uword pos = 0; pos < q1.n_elem; ++pos)
      {
	D( q1(pos) ) = THRESH;
      }
      cov_seg_i = V*diagmat(D)*V.t();  
      
    }  
    //end suggestion

     eig_sym(D, V, cov_seg_i);
     mat log_M = V*diagmat( log(D) )*V.t();
     
     #pragma omp critical
     {
     cov_seg_i.save( save_cov_seg.str(), hdf5_binary ); 
     log_M.save( save_LogMcov_seg.str(), hdf5_binary );
     }

      

  }




