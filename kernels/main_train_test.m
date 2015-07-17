clear all
clc
%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda

sigma = 0.1:0.2:1;

display('Training svm + Kernel Log-Euclidean Distance');
ACC = [];

for i=1:length(sigma)
   sigma(i)
   acc = kth_train_LogEuclidean(sigma(i));
   mean(acc)
   ACC = [ACC; acc];
    
end
    