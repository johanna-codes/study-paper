inline
opt_feat::opt_feat(const std::string in_path,
		   const std::string in_actionNames,  
		   const int in_col, 
		   const int in_row,
		   const int in_scale_factor, 
		   const int in_shift,
		   const int in_scene, //only for kth
		   const int in_dim
)
:path(in_path), actionNames(in_actionNames), col(in_col), row(in_row), scale_factor(in_scale_factor), shift(in_shift), total_scene(in_scene), dim(in_dim)
{
  
  actions.load( actionNames );  

}

inline
void
opt_feat::features_all_videos( field<string> all_people ) 
{
  int n_actions = actions.n_rows;
  int n_peo =  all_people.n_rows;
  //all_people.print("people");
  
  field <std::string> load_save_names (n_peo*n_actions,3); 
  int sc = total_scene; //Solo estoy usando 1 
  int k =0;
  
  for (int pe = 0; pe< n_peo; ++pe)
  {
    for (int act=0; act<n_actions; ++act)
    {
      std::stringstream ss_video_name;
      ss_video_name << path << actions (act) << "/" << all_people (pe) << "_" << actions (act) << "_d" << sc << "_uncomp.avi";
      
      std::stringstream save_folder;
      std::stringstream save_feat_video_i;
      std::stringstream save_labels_video_i;
      
      save_folder << "./kth-features_dim" << dim <<  "/sc" << sc << "/scale" << scale_factor << "-shift"<< shift ;
      save_feat_video_i   << save_folder.str() << "/" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
      save_labels_video_i << save_folder.str() << "/lab_" << all_people (pe) << "_" << actions(act) << "_dim" << dim  << ".h5";
      
      load_save_names(k,0) = ss_video_name.str();
      load_save_names(k,1) = save_feat_video_i.str();
      load_save_names(k,2) = save_labels_video_i.str();
      k++;
      
    }
  }
  
  //int nProcessors=omp_get_max_threads();
  //std::cout<<nProcessors<<std::endl;
  //std::cout<< omp_get_num_threads()<<std::endl;
  
  
  wall_clock timer;
  timer.tic();
  omp_set_num_threads(1); //Use only 8 processors
  
  #pragma omp parallel for 
  for (int i = 0; i<load_save_names.n_rows; ++i)
  {
    
    std::string one_video = load_save_names(i,0);
    int tid=omp_get_thread_num();
    
    #pragma omp critical
    cout<< "Processor " << tid <<" doing "<< one_video << endl;
    
    
    Struct_feat_lab my_Struct_feat_lab;
    
    feature_video( one_video, my_Struct_feat_lab );
    
    
    
    mat mat_features_video_i;
    vec lab_video_i;
    
    if (my_Struct_feat_lab.features_video_i.size()>0)
    {
      mat_features_video_i.zeros( dim,my_Struct_feat_lab.features_video_i.size() );
      lab_video_i.zeros( my_Struct_feat_lab.features_video_i.size() );
      for (uword i = 0; i < my_Struct_feat_lab.features_video_i.size(); ++i)
      {
	mat_features_video_i.col(i) = my_Struct_feat_lab.features_video_i.at(i)/norm(my_Struct_feat_lab.features_video_i.at(i),2);
	lab_video_i(i) = my_Struct_feat_lab.labels_video_i.at(i);
      }
    }
    else
    {
      mat_features_video_i.zeros(dim,0);
    }
    
    
    my_Struct_feat_lab.features_video_i.clear();
    my_Struct_feat_lab.labels_video_i.clear();
  
  
    std::string save_feat_video_i   = load_save_names(i,1);
    std::string save_labels_video_i = load_save_names(i,2);
    
    #pragma omp critical
    {
    mat_features_video_i.save( save_feat_video_i, hdf5_binary );
    lab_video_i.save( save_labels_video_i, hdf5_binary );
    }
  }
  
  double n = timer.toc();
  cout << "number of seconds in parallel : " << n << endl;
  
}

// //****************** Feature Extraction**************************************
// //***************************************************************************


inline 
void
opt_feat::feature_video( std::string one_video, Struct_feat_lab &my_Struct_feat_lab )
{
  
  
  
  my_Struct_feat_lab.features_video_i.clear();
  my_Struct_feat_lab.labels_video_i.clear();
  
  int new_row = row;
  int new_col = col;
  
  cv::VideoCapture capVideo(one_video);
  int n_frames = capVideo.get(CV_CAP_PROP_FRAME_COUNT);
  
  
  if( !capVideo.isOpened() )
  {
    cout << "Video couldn't be opened" << endl;
    return;
  }
  
  cv::Mat prevgray, gray, flow, cflow, frame, prevflow;
  cv::Mat ixMat, iyMat, ixxMat, iyyMat;
  cv::Mat flow_xy[2], mag, ang;
  
  
  string text;
  
  
  for(int fr=0; fr<n_frames; fr++){
    
    //cout << fr << " " ;
    
    bool bSuccess = capVideo.read(frame); // read a new frame from video
    
    if (!bSuccess) //if not success, break loop
    {
      cout << "Cannot read the frame from video file" << endl;
      break;
    }
    
    
    if (shift!=0)
    {
      
      int shif_x = floor(col*shift/100);
      int shif_y = floor(row*shift/100);
      
      frame = Shift_Image( frame, shif_x, shif_y);
      
    }
    
    
      if (scale_factor!=1)
    {
      new_row = row*scale_factor;
      new_col = col*scale_factor;
      cv::resize( frame, frame, cv::Size(new_row, new_col) );
    }
    
    
    
    
    
    
    cv::cvtColor(frame, gray, CV_BGR2GRAY);
    
    
    
    if( prevgray.data )
    {
      //cout << t << " " ;
      cv::calcOpticalFlowFarneback(prevgray, 
				   gray, 
				   flow, 
				   0.5, //pyr_scale
				   3,   //levels
				   9,   //winsize
				   1,   //iterations
				   5,   //poly_n
				   1.1, //poly_sigma
				   0);  //flags
      
      
      //optical flow
      cv::split(flow, flow_xy);
      cv::cartToPolar(flow_xy[0], flow_xy[1], mag, ang, true);
      
      cv::Sobel(gray, ixMat, CV_32F, 1, 0, 1);
      cv::Sobel(gray, iyMat, CV_32F, 0, 1, 1);
      cv::Sobel(gray, ixxMat, CV_32F, 2, 0, 1);
      cv::Sobel(gray, iyyMat, CV_32F, 0, 2, 1);
      
      float  ux = 0, uy = 0, vx = 0,  vy = 0;
      float u, v;
      
      if( prevflow.data )
      {
	//cout << new_col << " " << new_row << endl;
	
	for (int x = 0 ; x < new_col ; ++x ){
	  for (int y = 0 ; y < new_row ; ++y ) {
	    
	    vec features_one_pixel(dim);
	    u = flow.at<cv::Vec2f>(y, x)[0];
	    v = flow.at<cv::Vec2f>(y, x)[1];
	    
	    //cout << "x= " << x << " - y= " << y << endl;
	    // x grad
	    //cout << " x y grad" << endl;
	    float ix = ixMat.at<float>(y, x);
	    //cout << " y grad" << endl;
	    float iy = iyMat.at<float>(y, x);
	    
	    // grad direction &  grad magnitude. (Edges)
	    //cout << "grad direction &  grad magnitude" << endl;
	    float gd = std::atan2(std::abs(iy), std::abs(ix));
	    float gm = std::sqrt(ix * ix + iy * iy);
	    
	    // x second grad
	    //cout << "x y  second grad " << endl;
	    float ixx = ixxMat.at<float>(y, x);
	    // y second grad
	    float iyy = iyyMat.at<float>(y, x);
	    
	    //du/dt
	    float ut = u - prevflow.at<cv::Vec2f>(y, x)[0];
	    // dv/dt
	    float vt = v - prevflow.at<cv::Vec2f>(y, x)[1];
	    
	    
	    
	    if (x>0 && y>0 )
	    {
	      ux = u - flow.at<cv::Vec2f>(y, x - 1)[0];
	      uy = u - flow.at<cv::Vec2f>(y - 1, x)[0];
	      vx = v - flow.at<cv::Vec2f>(y, x - 1)[1];
	      vy = v - flow.at<cv::Vec2f>(y - 1, x)[1];
	    }
	    
	    
	    float Div = (ux + vy);
	    float Vor = (vx - uy);
	    //Adding more features
	    mat G (2,2);
	    mat S;
	    float gd_opflow = ang.at<float>(y,x);
	    float mg_opflow = mag.at<float>(y,x);
	    //Gradient Tensor
	    G   << ux << uy << endr
	    << vx << vy << endr;
	    
	    //Rate of Stein Tensor  
	    S = 0.5*(G + G.t());
	    
	    float tr_G = trace(G);
	    float tr_G2 = trace( square(G) );
	    float tr_S = trace(S);
	    float tr_S2 = trace(square(S));
	    
	    //Tensor Invariants  of the optical flow
	    float Gten = 0.5*( tr_G*tr_G - tr_G2 );
	    float Sten = 0.5*( tr_S*tr_S - tr_S2 ); 
	    
	    
	    
	    
// 	    if (dim ==12)
// 	    {
// 	      features_one_pixel  << x       << y       << fr   << gm  << u    << v 
// 	      << abs(ut) << abs(vt) << Div  << Vor << Gten << Sten;
// 	    }
	    
	   
	      features_one_pixel  << abs(ix) << abs(iy) << abs(ixx) 
	      << abs(iyy) << gm << gd <<  u << v << abs(ut) 
	      << abs(vt) << (ux + vy)  << (vx - uy);
	    
	    
	    
	    
	    if (!is_finite( features_one_pixel ) )
	    {
	      cout << "It's not FINITE... continue???" << endl;
	      getchar(); 
	    }
	    // Plotting Moving pixels
	    //cout << " " << gm;
	    
	    double is_zero = accu(abs(features_one_pixel));
	    
	    if (gm>40 && is_finite( features_one_pixel ) && is_zero!=0 ) // Empirically set to 40
	    {   
	      
	      frame.at<cv::Vec3b>(y,x)[0] = 0;
	      frame.at<cv::Vec3b>(y,x)[1] = 0;
	      frame.at<cv::Vec3b>(y,x)[2] = 255;
	      my_Struct_feat_lab.features_video_i.push_back(features_one_pixel);
	      my_Struct_feat_lab.labels_video_i.push_back( fr );
	    }
	  }
	}
      }
      
      if(cv::waitKey(30)>=0)
	break;
      
    }
    
    std::swap(prevgray, gray);
    std::swap(prevflow, flow);
    
    //cv::imshow("color", frame);
    //cv::waitKey();
    
    
  }
  
  
}


inline 
cv::Mat
opt_feat::Shift_Image( cv::Mat src_in, int num_pixels_x, int num_pixels_y)
{
  
  
  cv::Mat img_out;
  
  
  cv::Mat rot_mat = (cv::Mat_<double>(2,3) << 1, 0, num_pixels_x, 0, 1, num_pixels_y);
  warpAffine( src_in, img_out, rot_mat, src_in.size() );
  
  if (num_pixels_x>0) //Move right
  {   
    
    cv::Mat col = src_in.col(0);
    cv::Mat row = src_in.row(0);
    
    
    for (int i=0; i<abs(num_pixels_x); ++i)
    {
      col.col(0).copyTo(img_out.col(i));
      
    }
    
    for (int i=0; i<abs(num_pixels_y); ++i)
    {
      row.row(0).copyTo(img_out.row(i));
      //src_in.copyTo(img_out,crop);
    }
  }
  
  if (num_pixels_x<0) //Move right
  {   
    
    int w = src_in.size().width;
    int h = src_in.size().height;
    cv::Mat col = src_in.col(w-1);
    cv::Mat row = src_in.row(h-1);
    
    for (int i=w-abs(num_pixels_x) ; i<w; ++i)
    {
      col.col(0).copyTo(img_out.col(i));
      //row.row(0).copyTo(img_out.row(i));
    } 
    
    for (int i=h-abs(num_pixels_y) ; i<h; ++i)
    {
      //col.col(0).copyTo(img_out.col(i));
      row.row(0).copyTo(img_out.row(i));
    }
  }
  

  return img_out;
}
