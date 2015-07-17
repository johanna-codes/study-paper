clear all
clc
%addpath('/media/johanna/HD1T/Toolbox/libsvm-3.20/matlab'); --> Home
addpath('/home/johanna/toolbox/libsvm-3.20/matlab');% --> Wanda


path  = '~/codes/codes-git/study-paper/trunk/';
sigma = 0.1:0.1:1;

%display('Training svm + Kernel Log-Euclidean Distance');
%ACC = [];
% for i=1:length(sigma)
%    sigma(i)
%    acc = kth_train_LogEuclidean(path, sigma(i));
%    mean(acc)
%    ACC = [ACC; acc];
%     
% end

display('Testing svm + Kernel Log-Euclidean Distance');
ACC =[];
scale =1;
shift=0;
for i=1:length(sigma)
   sigma(i)
   acc = kth_test(path,scale, shift,sigma(i));
   mean(acc)
   ACC = [ACC; acc];
    
end