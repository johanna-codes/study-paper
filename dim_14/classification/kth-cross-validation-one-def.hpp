class kth_cv_omp //KTH Cross validation
{
  
  public:
    
    inline kth_cv_omp(const std::string in_path,
		const std::string in_actionNames,  
		const field<std::string> in_all_people,
		const int in_scale_factor, 
		const int in_shift,
		const int in_scene, //only for kth
		const int in_dim 
 		);
    inline void logEucl();
    inline void SteinDiv();
    inline float proj_grass(int p);
    inline float BC_grass(int p);

    
    
    const std::string path;
    const std::string actionNames;
    field<std::string> all_people;
    
    const int scale_factor;
    const int shift;
    const int total_scenes;
    const int dim;
    int p;

    
    field<std::string> actions;
    
    
    
  private:
    inline uword logEucl_one_video( int pe_test, std::string load_sub_path, std::string load_cov);
    inline uword SteinDiv_one_video( int pe_test, std::string load_sub_path, std::string load_cov);
    inline uword ProjectionMetric_one_video(int pe_test, std::string load_sub_path, std::string load_Gnp, int p);
    inline uword BinetCauchyMetric_one_video(int pe_test, std::string load_sub_path, std::string load_Gnp);

   
    
    //inline uword ProjectionMetric_one_seg_est_lab(int pe_test, std::string load_sub_path, std::string segm_name, int p);
    //inline uword BinetCauchyMetric_one_seg_est_lab(int pe_test, std::string load_sub_path, std::string segm_name);
    

    
    
};