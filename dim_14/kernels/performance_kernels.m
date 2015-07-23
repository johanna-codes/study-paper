clear all
close all
clc



%% ProjRGB Kernel
delta = -14:1:21;
projRGB = [32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667 32.6667   32.6667   32.6667   32.6667   33.3333   34.0000   38.0000   44.0000   50.6667   53.3333   53.3333   60.0000   59.3333   56.6667 48.6667   39.3333   32.0000   27.3333   23.3333   22.0000   20.6667   46.6667];
plot(delta,projRGB, '-rd', 'LineWidth',3,'MarkerSize',3)
title('Projection RGB Kernel','FontSize',30)
ylabel('(%)','FontSize',20)
xlabel('delta','FontSize',20)
set(gca,'FontSize',20);

%Home
print('/media/johanna/HD1T/latex-svn/study-paper-report/v2/images/projRGB_ker', '-dpng','-r300');


%% ProjPoly Kernel
figure
degree = 1:10;
projPoly = [32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667];
plot(degree,projPoly, '-bd', 'LineWidth',3,'MarkerSize',3);
title('Projection Polynomial Kernel','FontSize',30)
ylabel('(%)','FontSize',20)
xlabel('degree','FontSize',20)
set(gca,'FontSize',20); 

%Home
print('/media/johanna/HD1T/latex-svn/study-paper-report/v2/images/projPoly_ker', '-dpng','-r300');

%% Riemannian Kernel
figure
sigma = 0.1:0.1:1;
logEucl = [64.6667   76.6667   76.6667   76.6667   76.0000   76.0000   74.6667   72.0000   70.0000   68.6667];
plot(sigma,logEucl, '-gd', 'LineWidth',3,'MarkerSize',3);
title('Riemannian Kernel','FontSize',30)
ylabel('(%)','FontSize',20)
xlabel('sigma','FontSize',20)
set(gca,'FontSize',20); 
%Home
print('/media/johanna/HD1T/latex-svn/study-paper-report/v2/images/LogEucl_ker', '-dpng','-r300');

