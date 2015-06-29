class grass_metric
{
  
  public:
    
    inline grass_metric();
    
    inline double proj_metric(mat &Y1, mat &Y2, int p); //Projection metric
    inline double BC_metric(mat &Y1, mat &Y2); // Binet-Cauchy metric
    

    
  private:
    

    
    
};