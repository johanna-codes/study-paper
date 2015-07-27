clear all
clc
close all
%**********Using 12 features (Without x and %y)*****************


%% One Grassmann Point per video. NN
p=1:12;

PM = [56.0000   53.0000   51.0000   54.0000   74.0000   93.0000   96.0000   88.0000   80.0000   91.0000   74.0000   19.0000];
BC = [56.0000   53.0000   51.0000   54.0000   74.0000   93.0000   96.0000   88.0000   80.0000   91.0000   74.0000   31.0000];



plot(p,PM, '-gd', 'LineWidth',3,'MarkerSize',5);
hold on
plot(p,BC, '-ro', 'LineWidth',3,'MarkerSize',5);
legend('Projection Metric','Binet-Cauchy','Location', 'southwest');
%title('Grassmann Manifolds','FontSize',30)
ylabel('(%)','FontSize',20)
xlabel('subspace order','FontSize',20)
set(gca,'FontSize',20); 

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/Grass_varying_p_NN_dim12_no_xy', '-dpng','-r300');

%% One Grassmann Point per video. SVM (using distances)
 figure
 PM_svm = [ 32.0000   40.0000   27.3333   32.0000   26.0000   41.3333   48.0000   52.6667   28.6667   32.0000   32.6667   16.6667];
 BC_svm = [29.3333   44.0000   34.6667   27.3333   28.6667   48.0000   57.3333   54.6667   32.0000   38.6667   33.3333   16.6667];
 
m_pm = max(PM_svm);
m_bc = max(BC_svm);


 plot(p,PM_svm, '-cd', 'LineWidth',3,'MarkerSize',5);
 hold on
 plot(p,BC_svm, '-mo', 'LineWidth',3,'MarkerSize',5);
 legend('Projection Metric','Binet-Cauchy','Location', 'southwest');
 ylabel('(%)','FontSize',20)
 xlabel('subspace order','FontSize',20)
 set(gca,'FontSize',20); 

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/Grass_varying_p_SVM_dim12_no_xy', '-dpng','-r300');

%Nicta
%print('/home/johanna/latex-svn/study-paper-reports/v5/images/Grass_varying_p_SVM_dim12_no_xy', '-dpng','-r300');
 

%% Best performances
close all
all = [max(PM) max(BC); m_pm m_bc]; 

Xlabel = {'NN';'Dist. vectors'};

h =  bar(all);
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

ylim([0 109])
set(gca,'XTickLabel',Xlabel,'FontSize',20);

l{1}='ProjMet'; l{2}='BinCau';
legend(h,l);



%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/Grass_best_p_both_dim12_no_xy','-dpng','-r300');

%Nicta
print('/home/johanna/latex-svn/study-paper-reports/v5/images/Grass_best_p_both_dim12_no_xy', '-dpng','-r300');
% 

