clear all
clc
%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda

sigma = 0.2:0.2:1;

display('Training svm + Kernel Log-Euclidean Distance', sigma(1));
ACC = [];

for i=1:length(sigma)
    sigma(i)
   acc = kth_train_dist_LogEuclidean(sigma(i))
   ACC = [ACC; acc];
    
end
    