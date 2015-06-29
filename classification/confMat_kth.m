clear all
close all
clc

%% Riemannian Manifolds

logEucl_est  =load('./results_L20/Log_Eucl_est_labels.dat');
logEucl_real =load('./results_L20/Log_Eucl_real_labels.dat');
confMat = confMatGet(logEucl_real + 1, logEucl_est + 1); %My labels start at 0.
opt=confMatPlot('defaultOpt');
opt.className={'boxing', 'hclapping', 'hwaving', 'jogging', 'running','walking'};
opt.mode='both';
figure(1); confMatPlot(confMat, opt); %title('Log Euclidean Distance');

pause
Steindiv_est  =load('./results_L20/Stein_div_est_labels.dat');
Steindiv_real =load('./results_L20/Stein_div_real_labels.dat');
confMat = confMatGet(Steindiv_real + 1, Steindiv_est + 1); %My labels start at 0.
opt=confMatPlot('defaultOpt');
opt.className={'boxing', 'hclapping', 'hwaving', 'jogging', 'running','walking'};
opt.mode='both';
figure(2); confMatPlot(confMat, opt); %title('Stein Divergence Metric');




