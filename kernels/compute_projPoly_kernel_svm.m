function [K] = compute_proj_kernel_svm(X_1,X_2, fhandle, gamma,d)

K = zeros(size(X_1,3),size(X_2,3));

for x_1 = 1: size(X_1,3)
    for x_2 = 1:size(X_2,3)
          K(x_1,x_2) = feval(fhandle,X_1(:,:,x_1), X_2(:,:,x_2),gamma,d);
    end
end