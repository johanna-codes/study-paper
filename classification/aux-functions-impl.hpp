 inline
 aux_fun::aux_fun()
 {
   
 }
 
 inline
 mat
 aux_fun::log_matrix(mat& M)
 {
   eig_sym(D, V, M);
   
   cout << "D: " << D << endl;
   getchar();
   cout << "V: " << V << endl;
   getchar();
   cout << "M: " << M << endl;
   getchar();
   mat log_M = V*diagmat( log(D) )*V.t();
   
   cout << "log_M: " << log_M << endl;
   getchar();
   
   return log_M;
 }
 
 
 inline
 mat
 aux_fun::exp_matrix(mat& M)
 {
   eig_sym(D, V, M);
   mat exp_M = V*diagmat( exp(D) )*V.t();
   return exp_M;
 }
 
 
 inline
 mat
 aux_fun::sqrt_matrix(mat& M)
 {
   eig_sym(D, V, M);
   mat sqroot_M = V*diagmat( sqrt(D) )*inv(V);
   return sqroot_M;
 }
 