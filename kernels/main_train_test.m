clear all
clc
%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda


path  = '~/codes/codes-git/study-paper/trunk/';


%% LogEuclidean
% display('Training svm + Kernel Log-Euclidean Distance');
% sigma = 0.1:0.1:1;
% ACC_train = zeros(length(sigma),25);
% parfor i=1:length(sigma)
%    sigma(i);
%    acc = kth_train_LogEuclidean(path, sigma(i));
%    ACC_train(i,:) = acc;
%     
% end
% 
%  display('Testing svm + Kernel Log-Euclidean Distance');
%  
%  test_acc = zeros(length(sigma),1);
%  scale = 1;
%  shift = 0;
%  
%  parfor i=1:length(sigma)
%     %sigma(i);
%     acc = kth_test_LogEuclidean(path,scale, shift, sigma(i));
%     test_acc(i) = acc;     
%  end


%% Projection kernel: RBF
 display('Training svm + Projection RBF Kernel ');
 delta = -14:1:21;
 dim = 14;
 p = 1:14; 
 %p = 12;
 ACC_train = zeros(length(delta),25);
 all_p = cell(length(p),1);
 
 for j=1:length(p)
     in_p = p(j);
     X = sprintf('p = %d ', in_p);
     disp(X);
     
 parfor i=1:length(delta)
    acc = kth_train_ProjectionRBF(path, delta(i), dim, in_p);
    ACC_train(i,:) = acc;
 end
 
 all_p{j} = ACC_train;
 end


 
% display('Testing svm + Projection RBF Kernel');
% delta = -14:1:21;
% dim = 14;
% p = 1:14; 
% test_acc = zeros( length(delta) );
% all_p = cell(length(p),1);
% scale = 1;
% shift = 0;
% for j=1:length(p)
%     in_p = p(j)
% parfor i=1:length(delta )
%    acc = kth_test_ProjectionRBF(path,scale, shift, delta (i), dim, in_p);
%    test_acc(i) = acc;     
% end
% all_p{j} = test_acc;
% end

%% Projection Kernel: Poly
%   display('Training svm + Projection Poly Kernel ');
%   degree = 1:14;
%   dim = 14;
%   p = 12;
%   ACC_train = zeros(length(degree),25);
%   
%   parfor i=1:length(degree)
%      acc = kth_train_ProjectionPoly(path,  dim, p, degree(i));
%      ACC_train(i,:) = acc;
%   end
%   
%  display('Testing svm + Projection Poly Kernel');
%  degree = 1:50;
%  dim = 14;
%  p = 12; 
%  test_acc = zeros(length(degree ),1);
%  scale = 1;
%  shift = 0;
%  
%  parfor i=1:length(degree )
%     acc = kth_test_ProjectionPoly(path,scale, shift, dim, p, degree(i));
%     test_acc(i) = acc;     
%  end


