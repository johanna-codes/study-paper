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
xlabel('delta','FontSize',20)
set(gca,'FontSize',20);
legend('p=1', 'p=2', 'p=3' ,'p=4' ,'p=5', 'p=6', 'p=7', 'p=8', 'p=9', 'p=10', 'p=11' ,'p=12', 'p=13', 'p=14', 'Location','BestOutside'     );

%Home
print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/projRBF_ker_p', '-dpng','-r300');

%% ProjPoly Kernel
figure
load('projPoly_all_p_performance.mat');

plot(p,test_acc, '-d','LineWidth',3,'MarkerSize',3)
title('Projection RGB Kernel','FontSize',30)
ylabel('(%)','FontSize',20)
xlabel('# of subspaces','FontSize',20)
set(gca,'FontSize',20);

print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/projPoly_ker_p', '-dpng','-r300');
