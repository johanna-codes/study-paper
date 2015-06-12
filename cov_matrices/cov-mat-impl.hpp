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
cov_mat_kth::calculate( field<string> all_people, int  in_dim  )
{
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
	load_feat_video_i << load_folder.str() << "/" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".dat";
	load_labels_video_i << load_folder.str() << "/lab_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".dat";
	
	//For one Video
	cov_mat_kth::one_video(load_feat_video_i.str(),	 load_labels_video_i.str() );
	
	
	
	
      }
    }
  }
  
  
  
}


inline
void
cov_mat_kth::one_video( std::string load_feat_video_i,	std::string load_labels_video_i )
{
  mat mat_features_video_i;
  vec lab_video_i;
  
  mat_features_video_i.load( load_feat_video_i, raw_ascii );
  lab_video_i.load( load_labels_video_i, raw_ascii );
  int n_vec = lab_video_i.n_elem;
  int last = lab_video_i( n_vec - 1 );
  cout << last << endl;
  
  int s = 0;
  
  std::stringstream save_folder;
  save_folder << "./kth-cov-mat/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
     
     
  for (int l=2; l<last-segment_length; l = l+4 )
  {
    running_stat_vec<rowvec> stat_seg(true);
    //int k =0;
    
    cout << " " << l;


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
   

   std::stringstream save_feat_video_i;
   save_feat_video_i << save_folder.str() << "/cov_seg" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".dat";

    s++;
  //aca guardar el segmento y crear un txt con los nombres de los segmentos
  
   
    
  }
  
  
  std::stringstream save_seg;
  uvec total_seg = s;
  
   cout << "Loading.." << endl;
   save_seg << save_folder.str() << "/num_seg" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".dat";
   total_seg.save( save_seg.str() );

	

  
}




/*
running_stat_vec<rowvec> more_stats(true);

for(uword i=0; i<20; ++i)
  {
  sample = randu<rowvec>(3);
  
  sample(1) -= sample(0);
  sample(2) += sample(1);
  
  more_stats(sample);
  }*/