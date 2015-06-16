inline
cov_mat_kth::cov_mat_kth( const std::string in_path,
			  const std::string in_actionNames,  
			  const int in_scale_factor, 
			  const int in_shift,
			  const int in_scene, //only for kth
			  const int in_segment_length
)
:path(in_path), actionNames(in_actionNames), scale_factor(in_scale_factor), shift(in_shift), scene(in_scene), segment_length(in_segment_length)
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
  
  for (int sc = 1; sc<=1; ++sc) //scene
  {
    for (int pe = 0; pe< n_peo; ++pe)
    {
      for (int act=0; act<n_actions; ++act)
      {
	
	
	std::stringstream load_folder;
	std::stringstream load_feat_video_i;
	std::stringstream load_labels_video_i;
	
	
	cout << "Loading.." << endl;
	load_folder << path << "/kth-features/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
	load_feat_video_i << load_folder.str() << "/" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
	load_labels_video_i << load_folder.str() << "/lab_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
	
	//For one Video
	cov_mat_kth::one_video(load_feat_video_i.str(),	 load_labels_video_i.str(), sc, pe, act );
	//getchar();
	
      }
    }
  }
}


inline
void
cov_mat_kth::one_video( std::string load_feat_video_i,	std::string load_labels_video_i, int sc, int pe, int act )
{
  cout << load_feat_video_i << endl;
  mat mat_features_video_i;
  vec lab_video_i;
  
  mat_features_video_i.load( load_feat_video_i, hdf5_binary );
  lab_video_i.load( load_labels_video_i, hdf5_binary );
  int n_vec = lab_video_i.n_elem;
  int last = lab_video_i( n_vec - 1 );
  //cout << last << endl;
  
  int s = 0;
  
  std::stringstream save_folder;
  save_folder << "./kth-cov-mat/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
  
  
  for (int l=2; l<last-segment_length; l = l+4 )
  {
    running_stat_vec<rowvec> stat_seg(true);
    //int k =0;
    
    //cout << " " << l;
    
    
    for (int j=l; j<=segment_length+1; ++j)
    {
      //k++;
      uvec indices = find(lab_video_i == j);
      mat tmp_feat = mat_features_video_i.cols(indices);
      //cout << "row&col " << tmp_feat.n_rows << " & " << tmp_feat.n_cols << endl;
      for (int v=0; v < tmp_feat.n_cols; ++v)
      {
	vec sample = tmp_feat.col(v);
	stat_seg (sample);
	
      }
      
      
    }
    
    
    std::stringstream save_cov_seg;
    save_cov_seg << save_folder.str() << "/cov_seg" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
    
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
    
    
    cov_seg_i.save( save_cov_seg.str(), hdf5_binary ); 
    
    
    s++;
    
    
  }
  
  
  std::stringstream save_seg;
  vec total_seg; 
  total_seg.zeros(1);
  total_seg( 0 ) = s;
  save_seg << save_folder.str() << "/num_seg_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".dat";
  total_seg.save( save_seg.str(), raw_ascii );
  
}


