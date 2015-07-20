clear all
clc
close all
p=1:14;

PM = [37.3333   40.6667   36.6667   38.0000   38.6667   41.3333   67.3333   60.6667   62.6667   55.3333   67.3333   54.6667   52.0000   18.0000];
BC = [37.3333   40.6667   36.6667   38.6667   38.6667   41.3333   67.3333   60.6667   62.6667   55.3333   67.3333   54.6667   52.0000   15.3333];


plot(p,PM, '-gd', 'LineWidth',3,'MarkerSize',5);
hold on
plot(p,BC, '-ro', 'LineWidth',3,'MarkerSize',5);
%title('Grassmann Manifolds','FontSize',30)
ylabel('(%)','FontSize',20)
xlabel('subspace order','FontSize',20)
set(gca,'FontSize',20); 

%Home
print('/media/johanna/HD1T/latex-svn/study-paper-report/v2/images/Grass_varying_p', '-dpng','-r300');