class opt_feat
{
public:  
    inline opt_feat(const std::string in_path,
		    const std::string in_actionNames,  
		    const int in_col, 
		    const int in_row,
		    const int in_scale_factor, 
		    const int in_shift,
		    const int in_scene
                  );
    
    inline void features_all_videos( field<string>all_people);


    

const std::string path;
const std::string actionNames;

const int col;
const int row;
const int scale_factor;
const int shift;
const int scene;
int dim;


vector <vec> features_video_i; //All vectors per vide_i are stored here

field<std::string> actions;


  private:
    inline void feature_video( std::string one_video );

 
  
};