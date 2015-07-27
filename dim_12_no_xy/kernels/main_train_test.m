%Kernel Learning for Extrinsic Classification of Manifold Features"
clear all
clc
%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda


path  = '~/codes/codes-git/study-paper/trunk/dim_12_no_xy/';
dim =12;

 
 %%%%%%%%%%%%%%%%%%%%   Grassmann Kernels %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%% Projection kernel: RBF
%  display('Training svm + Projection RBF Kernel ');
%  delta = -14:1:21;
%  p = 1:dim; 
% 
%  ACC_train = zeros(length(delta),25);
%  all_p = cell(length(p),1);
%  
%  
%  for j=1:length(p)
%      in_p = p(j);
%      X = sprintf('p = %d ', in_p);
%      disp(X);
%      
%  parfor i=1:length(delta)
%     acc = kth_train_ProjectionRBF(path, delta(i), dim, in_p);
%     ACC_train(i,:) = acc;
%  end
%  
%  all_p{j} = ACC_train;
%  end

 
display('Testing svm + Projection RBF Kernel');
delta = -14:1:21;
p = 1:dim; 
test_acc = zeros( length(delta),1 );
all_p = cell(length(p),1);
scale = 1;
shift = 0;
for j=1:length(p)
    in_p = p(j)
parfor i=1:length(delta )
   acc = kth_test_ProjectionRBF(path,scale, shift, delta (i), dim, in_p);
   test_acc(i) = acc;     
end
all_p{j} = test_acc;
end

save_results =strcat( 'projRBF_all_p_delta_performance_dim12.mat');
save(save_results, 'all_p', 'delta', 'p', 'dim', 'scale', 'shift');
    
    

%% Projection Kernel: Poly
 display('Training svm + Projection Poly Kernel ');
 p = 1:dim;
 ACC_train = zeros(length(p),25);
 
  
  parfor i=1:length(p)
     acc = kth_train_ProjectionPoly(path,  dim, p(i));
     ACC_train(i,:) = acc;
  end
 
  
  
  
 display('Testing svm + Projection Poly Kernel');
 p = 1:dim;
 test_acc = zeros(length(p),1);
 scale = 1;
 shift = 0;
 
 parfor i=1:length( p )
    acc = kth_test_ProjectionPoly(path,scale, shift, dim, p(i) );
    test_acc(i) = acc;     
 end

save_results =strcat( 'projPoly_all_p_performance_dim12.mat');
save(save_results, 'test_acc', 'p', 'dim', 'scale', 'shift');

