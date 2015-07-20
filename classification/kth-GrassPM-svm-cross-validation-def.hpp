class kth_cv_svm_Grass_PM //KTH Cross validation
{
  
  public:
    
    inline kth_cv_svm_Grass_PM(const std::string in_path,
		const std::string in_actionNames,  
		const field<std::string> in_all_people,
		const int in_scene, //only for kth
		const int in_dim 
 		);   
    
    inline void train(int p, int ts_scale, int ts_shift);
    inline float test(int p, int ts_scale, int ts_shift);

    
    const std::string path;
    const std::string actionNames;
    field<std::string> all_people;
    
    const int total_scenes;
    const int dim;
    int p;

    
    field<std::string> actions;
    
    
    
  private:
    
    inline void distances(int scale_factor, int shift);
    inline void svm_train();
    inline vec dist_one_video(int pe_test, std::string load_sub_path, std::string load_cov);
    
    
   
    
    
};