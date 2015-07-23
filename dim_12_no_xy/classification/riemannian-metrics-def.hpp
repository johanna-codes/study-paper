class ri_metrics
{
  
  public:
    
    inline ri_metrics();
    
    inline double logEucl(mat& A, mat& B); // Log-Euclidean
    inline double airm(mat& A, mat& B); // Affine Invariant Riemannian Metric
    inline double dStein(mat& A, mat& B); // Direct Stein Divergence
    inline double rStein(mat& A, mat& B); // Relational Stein Divergence
    
  private:
  
    
    
};