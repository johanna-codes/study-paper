class aux_fun
{
  
  public:
    
    inline aux_fun();
    
    inline mat log_matrix(mat& M);
    inline mat exp_matrix(mat& M);
    inline mat sqrt_matrix(mat& M); // Square root
    
  private:
    
    mat V; 
    vec D; 
    
    
};