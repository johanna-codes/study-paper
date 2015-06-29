 //Grassmann Discriminant Analysis: a Unifying View on Subspace-Based Learning
 inline
 grass_metric::grass_metric()
 {
   
 }
 
 inline
 double
 grass_metric::proj_metric(mat &Y1, mat &Y2, int p)
 {
   
   vec cos_theta;
   mat X = Y1.t()*Y2;
   svd( cos_theta, X );
   
   vec cos2_theta = square(cos_theta);
   double tmp_sum = sum(cos2_theta);   
   double dist = sqrt (p - tmp_sum);
   
   return dist;
   
 }
 
 
 inline
 double
 grass_metric::BC_metric(mat &Y1, mat &Y2)
 {
   
   vec cos_theta;
   mat X = Y1.t()*Y2;
   svd( cos_theta, X );
   
   vec cos2_theta = square(cos_theta);
   double tmp_mul = prod(cos2_theta);   
   double dist = sqrt (1 - tmp_mul);
   
   return dist;
   
 }