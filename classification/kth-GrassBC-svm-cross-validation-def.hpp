class kth_cv_svm_Grass_BC //KTH Cross validation
{
  
  public:
    
    inline kth_cv_svm_Grass_BC(const std::string in_path,
		const std::string in_actionNames,  
		const field<std::string> in_all_people,
		const int in_scale_factor, 
		const int in_shift,
		const int in_scene, //only for kth
		const int in_dim 
 		);   
    
    inline void run( );

    
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
    
    inline void distances();
    inline void CV();
    inline vec dist_one_video(int pe_test, std::string load_sub_path, std::string load_cov);
    
    
   
    
    
};