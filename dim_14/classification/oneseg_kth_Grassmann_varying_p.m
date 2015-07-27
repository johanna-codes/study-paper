clear all
clc
close all

%% One Grassmann Point per video. NN
p=1:14;

PM = [37.3333   40.6667   36.6667   38.0000   38.6667   41.3333   67.3333   60.6667   62.6667   55.3333   67.3333   54.6667   52.0000   18.0000];
BC = [37.3333   40.6667   36.6667   38.6667   38.6667   41.3333   67.3333   60.6667   62.6667   55.3333   67.3333   54.6667   52.0000   15.3333];


plot(p,PM, '-gd', 'LineWidth',3,'MarkerSize',5);
hold on
plot(p,BC, '-ro', 'LineWidth',3,'MarkerSize',5);
legend('Projection Metric','Binet-Cauchy','Location', 'southwest');
%title('Grassmann Manifolds','FontSize',30)
ylabel('(%)','FontSize',20)
xlabel('subspace order','FontSize',20)
set(gca,'FontSize',20); 

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/Grass_varying_p_NN', '-dpng','-r300');

%% One Grassmann Point per video. SVM
figure
PM_svm = [ 30.0000   34.0000   38.0000   36.6667   35.3333   30.6667   38.0000   50.6667   47.3333   46.0000   27.3333   34.0000   32.6667   16.6667];
BC_svm = [ 34.0000   38.6667   40.0000   38.6667   40.6667   26.6667   26.0000   54.6667   56.6667   53.3333   35.3333   32.6667   38.0000   16.6667];
plot(p,PM_svm, '-cd', 'LineWidth',3,'MarkerSize',5);
hold on
plot(p,BC_svm, '-mo', 'LineWidth',3,'MarkerSize',5);
legend('Projection Metric','Binet-Cauchy','Location', 'southwest');
ylabel('(%)','FontSize',20)
xlabel('subspace order','FontSize',20)
set(gca,'FontSize',20); 
%
% %Home
% print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/Grass_varying_p_SVM', '-dpng','-r300');
% 
