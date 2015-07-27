clear all
close all
clc



%% ProjRBF Kernel
load('projRBF_all_p_delta_performance.mat');
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
legend('m=1', 'm=2', 'm=3' ,'m=4' ,'m=5', 'm=6', 'm=7', 'm=8', 'm=9', 'm=10', 'm=11' ,'m=12', 'm=13', 'm=14', 'Location','BestOutside'     );

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/projRBF_ker_p', '-dpng','-r300');

%% ProjPoly Kernel
figure
load('projPoly_all_p_performance.mat');
max_projPoly = max(test_acc);
plot(p,test_acc, '-d','LineWidth',3,'MarkerSize',3)
ylabel('(%)','FontSize',20)
xlabel('# of subspaces','FontSize',20)
set(gca,'FontSize',20);

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/projPoly_ker_p', '-dpng','-r300');

%% LogEuclidean Kernel
figure
load('LogEucl_all_sigma_performance.mat');
max_LogEucl = max(test_acc);
plot(sigma,test_acc, '-gd','LineWidth',3,'MarkerSize',3)
ylabel('(%)','FontSize',20)
xlabel('\sigma','FontSize',20)
set(gca,'FontSize',20);

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/LogEucl_ker_sigma', '-dpng','-r300');

%% LED-RBF Kernel
figure
load('LED_RBF_all_delta_performance.mat')
max_ledRBF= max(test_acc);
plot(delta,test_acc, '-md','LineWidth',3,'MarkerSize',3)
ylabel('(%)','FontSize',20)
xlabel('\delta','FontSize',20)
set(gca,'FontSize',20);

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/ledRBF_ker_delta', '-dpng','-r300');


%% LED-POLY Kernel
figure
load('LED_POLY_all_n_performance.mat')
max_ledPOLY= max(test_acc);
plot(test_acc, '-bd','LineWidth',3,'MarkerSize',3)
ylabel('(%)','FontSize',20)
xlabel('D','FontSize',20)
set(gca,'FontSize',20);

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/ledPoly_ker_delta', '-dpng','-r300');

%Nicta
%print('/home/johanna/latex-svn/study-paper-reports/v5/images/ledPoly_ker_delta', '-dpng','-r300');

%% ALL Best Perfomances
close all
all = [max_projRBF max_projPoly max_LogEucl max_ledRBF max_ledPOLY];


Xlabel = {'projRBF';'projPoly'; 'LogEucl'; 'LED-RBF'; 'LED-POLY'};

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
set(gca,'XTickLabel',Xlabel,'FontSize',15);

%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/summary_ker','-dpng','-r300');

%Nicta
%print('/home/johanna/latex-svn/study-paper-reports/v5/images/summary_ker', '-dpng','-r300');


% l{1}='ProjMet'; l{2}='BinCau';
% legend(h,l);