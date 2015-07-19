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


%% Projection kernel
 display('Training svm + Projection RBF Kernel ');
 delta = -14:1:21;
 n = 14;
 ACC_train = zeros(length(delta),25);
 
 parfor i=1:length(delta)
    sigma(i);
    acc = kth_train_ProjectionRBF(path, delta(i), n);
    ACC_train(i,:) = acc;
 end


 
%  display('Testing svm + Projection RBF Kernel');
%  
%  test_acc = zeros(length(gamma),1);
%  scale = 1;
%  shift = 0;
%  
%  parfor i=1:length(sigma)
%     acc = kth_train_ProjectionRBF(path,scale, shift, gamma(i));
%     test_acc(i) = acc;     
%  end


