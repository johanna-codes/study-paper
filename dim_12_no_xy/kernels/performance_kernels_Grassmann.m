clear all
close all
clc



%% ProjRBF Kernel
load('projRBF_all_p_delta_performance_dim12.mat');
all_p_mat = zeros(length(delta), length(all_p));

for i=1: length(all_p)
  all_p_mat(:,i) = all_p{i};
end

max_projRBF = max(max(all_p_mat));
plot(delta,all_p_mat, '-d','LineWidth',3,'MarkerSize',3)
%min_max = [min(delta), max(delta)+1];
xlim([min(delta), max(delta)]);
ylabel('(%)','FontSize',20)
xlabel('\delta','FontSize',20)
set(gca,'FontSize',20);
legend('m=1', 'm=2', 'm=3' ,'m=4' ,'m=5', 'm=6', 'm=7', 'm=8', 'm=9', 'm=10', 'm=11' ,'m=12', 'Location',  'WestOutside');

%NICTA
%print('/home/johanna/latex-svn/study-paper-reports/v5/images/projRBF_ker_p_dim12', '-dpng','-r300');

%% ProjPoly Kernel
figure
load('projPoly_all_p_performance_dim12.mat');
max_projPoly = max(test_acc);
plot(p,test_acc, '-d','LineWidth',3,'MarkerSize',3)
ylabel('(%)','FontSize',20)
xlabel('# of subspaces','FontSize',20)
set(gca,'FontSize',20);

%NICTA
%print('/home/johanna/latex-svn/study-paper-reports/v5/images/projPoly_ker_p_dim12', '-dpng','-r300');


%% Comparing Best ones
close all
all_per = [ max_projRBF; max_projPoly];
colormap('summer');



h =  bar(all_per);
colormap('summer');
ybuff=2;
for i=1:length(h)
    XDATA=get(get(h(i),'Children'),'XData');
    YDATA=get(get(h(i),'Children'),'YData');
    for j=1:size(XDATA,2)
        x=XDATA(1,j)+(XDATA(3,j)-XDATA(1,j))/2;
        y=YDATA(2,j)+ybuff;
        t=[num2str(YDATA(2,j),3) ,'%'];
        text(x,y,t,'Color','k','HorizontalAlignment','left','Rotation',90,'FontSize',15 )
    end
end
Xlabel = {'Proj-RBF kernel';  'Proj-Poly kernel'};
ylim([0 85])
set(gca,'XTickLabel',Xlabel,'FontSize',20);



print('/home/johanna/latex-svn/study-paper-reports/v5/images/ker_Grass_best_p_both_dim12_no_xy','-dpng','-r300');

