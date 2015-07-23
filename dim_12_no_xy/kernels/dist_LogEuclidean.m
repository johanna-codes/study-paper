function dist_g = dist_LogEuclidean(X,Y)

dist_g = norm(logm(X) - logm(Y),'fro');
