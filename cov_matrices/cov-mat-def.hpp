class cov_mat_kth
{
public:  
    inline cov_mat_kth(const std::string in_path,
		    const std::string in_actionNames,  
		    const int in_scale_factor, 
		    const int in_shift,
		    const int in_scene,
		    const int segment_length
                  );
    


    inline void calculate( field<string> all_people, int dim );
    inline void calculate_one_per_video( field<string> all_people, int dim );


const std::string path;
const std::string actionNames;

const int scale_factor;
const int shift;
const int total_scenes;
const int segment_length;
int dim;

field<std::string> actions;
field<string> all_people;


  private: 
    inline void one_video( std::string load_feat_video_i, std::string load_labels_video_i, int sc, int pe, int act );
    inline void one_video_one_cov( std::string load_feat_video_i, std::string load_labels_video_i, int sc, int pe, int act );


 
  
};