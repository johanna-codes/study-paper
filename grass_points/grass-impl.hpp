inline
grass_points::grass_points( const std::string in_path,
			    const std::string in_actionNames,  
			    const int in_scale_factor, 
			    const int in_shift,
			    const int in_scene, //only for kth
			    const int in_segment_length,
			    const int in_p
)
:path(in_path), actionNames(in_actionNames), scale_factor(in_scale_factor), shift(in_shift), total_scenes(in_scene), segment_length(in_segment_length), p(in_p)
{
  actions.load( actionNames );  
}


inline
void
grass_points::calculate( field<string> in_all_people, int  in_dim  )
{
  all_people = in_all_people;
  dim = in_dim;
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  //all_people.print("people");
  
  for (int sc = 1; sc<=total_scenes; ++sc) //scene
  {
    for (int pe = 0; pe< n_peo; ++pe)
    {
      for (int act=0; act<n_actions; ++act)
      {
	
	
	std::stringstream load_folder;
	std::stringstream load_feat_video_i;
	std::stringstream load_labels_video_i;
	
	
	//cout << "Loading.." << endl;
	load_folder << path << "/kth-features/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
	load_feat_video_i << load_folder.str() << "/" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
	load_labels_video_i << load_folder.str() << "/lab_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
	
	//For one Video
	if(  !( ( sc==3 && pe==12) && act==1) ) //person13_handclapping_d3 doesn't exist
	{
	  one_video(load_feat_video_i.str(),	 load_labels_video_i.str(), sc, pe, act );
	}
	//getchar();
	
      }
    }
  }
}


inline
void
grass_points::one_video( std::string load_feat_video_i,	std::string load_labels_video_i, int sc, int pe, int act )
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
  save_folder << "./kth-grass-point/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
  
  
  for (int l=2; l<last-segment_length; l = l+4 )
  {
    std::vector<vec> feat_seg;
    
    for (int j=l; j< l + segment_length; ++j)
    {
      //k++;
      //cout << " " << j;
      uvec indices = find(lab_video_i == j);
      mat feat_fr = mat_features_video_i.cols(indices);
      
      for (int v=0; v < feat_fr.n_cols; ++v)
      {
	vec sample = feat_fr.col(v);
	feat_seg.push_back(sample);
      }

    }
    cout << feat_seg.size() << endl;
    if (feat_seg.size()>100) // Cuando en el segmento hay mas de 100 vectores 
    {
      mat mat_feat_seg(dim,feat_seg.size());
      
      for (uword i = 0; i < feat_seg.size(); ++i)
      {
	mat_feat_seg.col(i) = feat_seg.at(i);
	
      }
      
      mat U; vec s;   mat V;
      svd(U,s,V,mat_feat_seg); 
      mat Gnp = U.cols(0,p-1);

      std::stringstream save_Gnp;
      save_Gnp << save_folder.str() << "/grass_pt" << s << "_"<< all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
      cout << save_Gnp.str() << endl;
      Gnp.save( save_Gnp.str(), hdf5_binary ); 
      
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
  total_seg.save( save_seg.str(), raw_ascii );
  cout << "Total # of segments " << s << endl;
  //cout << "press a key " ;
  //getchar();
  
}


