clear all
%close all
clc

vec_shift = [-25 -20 -15  -10 -5  0  5  10 15 20 25];
n = length(vec_shift);
LogEucl  = zeros(1,n);
SteinDiv = zeros(1,n);
GrassPM  = zeros(1,n);
GrassBC  = zeros(1,n);

for i=1:n
   
    tmp_est =  load( strcat('./svm_results_2/LogEucl_scale1-shift', num2str(vec_shift(i)), '_est_labels.dat' ) );    
    tmp_real= load( strcat('./svm_results_2/LogEucl_scale1-shift', num2str(vec_shift(i)), '_real_labels.dat') );
    tmp_acc = length(find(tmp_est==tmp_real))/length(tmp_est)*100;
    LogEucl(i) = tmp_acc;
    
    tmp_est =  load( strcat('./svm_results_2/SteinDiv_scale1-shift', num2str(vec_shift(i)), '_est_labels.dat') );    
    tmp_real= load( strcat('./svm_results_2/SteinDiv_scale1-shift', num2str(vec_shift(i)), '_real_labels.dat') );
    tmp_acc = length(find(tmp_est==tmp_real))/length(tmp_est)*100;
    SteinDiv(i) = tmp_acc;
    
    tmp_est =  load( strcat('./svm_results_2/Grass_PM_scale1-shift', num2str(vec_shift(i)), '_est_labels.dat') );    
    tmp_real= load( strcat('./svm_results_2/Grass_PM_scale1-shift', num2str(vec_shift(i)), '_real_labels.dat') );
    tmp_acc = length(find(tmp_est==tmp_real))/length(tmp_est)*100;
    GrassPM(i) = tmp_acc;
    
    tmp_est =  load( strcat('./svm_results_2/Grass_BC_scale1-shift', num2str(vec_shift(i)), '_est_labels.dat') );    
    tmp_real= load( strcat('./svm_results_2/Grass_BC_scale1-shift', num2str(vec_shift(i)), '_real_labels.dat') );
    tmp_acc = length(find(tmp_est==tmp_real))/length(tmp_est)*100;
    GrassPM(i) = tmp_acc;

end

all = [LogEucl; SteinDiv; GrassPM; GrassPM];
plot(vec_shift,all', '-d', 'LineWidth',3,'MarkerSize',10);

legend('LogEucl','SteinDiv', 'Projection Metric', 'Binet-Cauchy', 'FontSize',30,'FontWeight','bold', 'Location','northeast');
set(gca,'XTickLabel', vec_shift, 'FontSize',30); % Change x-axis ticks labels to desired values.
xlabel('(%)')


