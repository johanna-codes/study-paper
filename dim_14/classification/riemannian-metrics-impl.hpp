
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
   
  
	     
	     
   double dist;
   
   aux_fun fun;
   
   
   logA = fun.log_matrix(A);
   logB = fun.log_matrix(B);
   
    //logA.print("logA");
    //logB.print("logB");
    
   dist = norm( logA - logB, "fro");
   
   return dist;
   
   
 }