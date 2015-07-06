class kth_cv_svm //KTH Cross validation
{
  
  public:
    
    inline kth_cv_svm(const std::string in_path,
		const std::string in_actionNames,  
		const field<std::string> in_all_people,
		const int in_scale_factor, 
		const int in_shift,
		const int in_scene, //only for kth
		const int in_dim 
 		);   
    
    inline void logEucl();

    
    //inline void SteinDiv();
    //inline void proj_grass(int p);
    //inline void BC_grass();

    
    
    const std::string path;
    const std::string actionNames;
    field<std::string> all_people;
    
    const int scale_factor;
    const int shift;
    const int total_scenes;
    const int dim;

    
    field<std::string> actions;
    
    
    
  private:
    inline void logEucl_distances();
    inline vec dist_logEucl_one_video(int pe_test, std::string load_sub_path, std::string load_cov);
    
    
    //inline uword logEucl_one_video( int pe_test, std::string load_sub_path, std::string load_cov);
    //inline uword SteinDiv_one_video( int pe_test, std::string load_sub_path, std::string load_cov);
    //inline uword ProjectionMetric_one_video(int pe_test, std::string load_sub_path, std::string load_Gnp, int p);
    //inline uword BinetCauchyMetric_one_video(int pe_test, std::string load_sub_path, std::string load_Gnp);

   
    
    
};