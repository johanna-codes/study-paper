
 inline
 ri_metrics::ri_metrics()
 {
   
 }
 
 inline
 double
 ri_metrics::logEucl(mat& A, mat& B)
 {
   mat logA;
   mat logB;
   
   A.print("test_cov");
   B.print("train_cov");
	     
	     
   double dist;
   
   aux_fun fun;
   
   
   logA = fun.log_matrix(A);
   logB = fun.log_matrix(B);
    
   dist = norm( logA - logB, "fro");
   
   return dist;
   
   
 }