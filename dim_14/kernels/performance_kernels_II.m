clear all
close all
clc
close all



%% ProjRBF Kernel
load('projRBF_all_p_delta_performance.mat');
all_p_mat = zeros(length(delta), length(all_p));

for i=1: length(all_p)
  all_p_mat(:,i) = all_p{i};
end


plot(delta,all_p_mat, '-d','LineWidth',3,'MarkerSize',3)
%title('Projection RBF Kernel','FontSize',30)
%min_max = [min(delta), max(delta)+1];
xlim([min(delta), max(delta)]);
ylabel('(%)','FontSize',20)
xlabel('\delta','FontSize',20)
set(gca,'FontSize',20);
legend('m=1', 'm=2', 'm=3' ,'m=4' ,'m=5', 'm=6', 'm=7', 'm=8', 'm=9', 'm=10', 'm=11' ,'m=12', 'm=13', 'm=14', 'Location','BestOutside'     );

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/projRBF_ker_p', '-dpng','-r300');

%% ProjPoly Kernel
figure
load('projPoly_all_p_performance.mat');

plot(p,test_acc, '-d','LineWidth',3,'MarkerSize',3)
% title('Projection RGB Kernel','FontSize',30)
ylabel('(%)','FontSize',20)
xlabel('# of subspaces','FontSize',20)
set(gca,'FontSize',20);

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/projPoly_ker_p', '-dpng','-r300');

%% LogEuclidean Kernel
figure
load('LogEucl_all_sigma_performance.mat');

plot(sigma,test_acc, '-gd','LineWidth',3,'MarkerSize',3)
%title('LogEuclidean Kernel Kernel','FontSize',30)
ylabel('(%)','FontSize',20)
xlabel('\sigma','FontSize',20)
set(gca,'FontSize',20);

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/LogEucl_ker_sigma', '-dpng','-r300');

%% LED-RBF Kernel
figure
load('LED_RBF_all_delta_performance.mat')

plot(delta,test_acc, '-md','LineWidth',3,'MarkerSize',3)
%title('LogEuclidean Kernel Kernel','FontSize',30)
ylabel('(%)','FontSize',20)
xlabel('\delta','FontSize',20)
set(gca,'FontSize',20);

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/LogEucl_ker_sigma', '-dpng','-r300');