clear all
close all
clc

%Results for Overlapping Segments
ACC = [];
%% Riemannian Manifolds

logEucl_est  =load('./results_L20/Log_Eucl_est_labels.dat');
logEucl_real =load('./results_L20/Log_Eucl_real_labels.dat');
confMat = confMatGet(logEucl_real + 1, logEucl_est + 1); %My labels start at 0.
opt=confMatPlot('defaultOpt');
opt.className={'box', 'hclap', 'hwave', 'jog', 'run','walk'};
opt.mode='both';
figure(1); 
confMatPlot(confMat, opt); 
%TitleH = title('Log Euclidean Distance');
%set(TitleH, 'Position', [0, 0],'VerticalAlignment', 'bottom',  'HorizontalAlignment', 'center')
%figure(1, 'Name','Log Euclidean Distance');
acc = length(find(logEucl_est==logEucl_real))/length(logEucl_est)*100;

ACC = [ACC acc];

% Improving image quality and saving
width = 4;     % Width in inches
height = 4.5;    % Height in inches
alw = 0.75;    % AxesLineWidth
fsz = 10;      % Fontsize
lw = 1.5;      % LineWidth
msz = 8;       % MarkerSize

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);

% Save the file as PNG
%print('/home/johanna/latex-svn/latex_rioux/reports/study-paper/images/CM_Log_Eucl','-dpng','-r300');



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

Steindiv_est  =load('./results_L20/Stein_div_est_labels.dat');
Steindiv_real =load('./results_L20/Stein_div_real_labels.dat');
confMat = confMatGet(Steindiv_real + 1, Steindiv_est + 1); %My labels start at 0.
opt=confMatPlot('defaultOpt');
opt.className={'box', 'hclap', 'hwave', 'jog', 'run','walk'};
opt.mode='both';
%figure(2); 
figure('name','Stein Divergence Metric');
confMatPlot(confMat, opt); %title('Stein Divergence Metric');

acc = length(find(Steindiv_est==Steindiv_real))/length(Steindiv_est)*100;
ACC = [ACC acc];

% Improving image quality and saving
width = 4;     % Width in inches
height = 4.5;    % Height in inches
alw = 0.75;    % AxesLineWidth
fsz = 10;      % Fontsize
lw = 1.5;      % LineWidth
msz = 8;       % MarkerSize

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);

% Save the file as PNG
%print('/home/johanna/latex-svn/latex_rioux/reports/study-paper/images/CM_Stein_Div','-dpng','-r300');



%% Grassmann Manifolds

PM_est  =load('./results_L20/grass_PM_est_labels.dat');
PM_real =load('./results_L20/grass_PM_real_labels.dat');
confMat = confMatGet(PM_real + 1, PM_est + 1); %My labels start at 0.
opt=confMatPlot('defaultOpt');
opt.className={'box', 'hclap', 'hwave', 'jog', 'run','walk'};
opt.mode='both';
%figure(3); 
figure('name','Projection Metric');
confMatPlot(confMat, opt); %title('Projection Metric');
acc = length(find(PM_est==PM_real))/length(PM_est)*100;
ACC = [ACC acc];

% Improving image quality and saving
width = 4;     % Width in inches
height = 4.5;    % Height in inches
alw = 0.75;    % AxesLineWidth
fsz = 10;      % Fontsize
lw = 1.5;      % LineWidth
msz = 8;       % MarkerSize

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);

% Save the file as PNG
%print('/home/johanna/latex-svn/latex_rioux/reports/study-paper/images/CM_grass_PM','-dpng','-r300');



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

BC_est  =load('./results_L20/grass_BC_est_labels.dat');
BC_real =load('./results_L20/grass_BC_real_labels.dat');
confMat = confMatGet(BC_real + 1, BC_est + 1); %My labels start at 0.
opt=confMatPlot('defaultOpt');
opt.className={'box', 'hclap', 'hwave', 'jog', 'run','walk'};
opt.mode='both';
figure('name','Binet Cauchy Metric');
%figure(4); 
confMatPlot(confMat, opt); %title('Binet Cauchy Metric');
acc = length(find(BC_est==BC_real))/length(BC_est)*100;
ACC = [ACC acc];


% Improving image quality and saving
width = 4;     % Width in inches
height = 4.5;    % Height in inches
alw = 0.75;    % AxesLineWidth
fsz = 10;      % Fontsize
lw = 1.5;      % LineWidth
msz = 8;       % MarkerSize

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);

% Save the file as PNG
%print('/home/johanna/latex-svn/latex_rioux/reports/study-paper/images/CM_grass_BC','-dpng','-r300');


close all


%%BAR with all performance 
figure
labels = {'Log_Eucl';'Stein_Div';'Proj-Metric';'Binet-Cauchy'};
xname = strtrim(cellstr(num2str(ACC'))') 
bar(ACC, 'm')
text(1:numel(ACC),ACC,xname,'horizontalalignment','center','verticalalignment','bottom') 
ylim([0 90])
set(gca,'XTickLabel',labels,'FontSize',15 );
ylabel('Recognition Accuracy (%)')
%xlabel('Metric Employed')






width = 8;     % Width in inches
height = 5;    % Height in inches
alw = 0.75;    % AxesLineWidth
fsz = 10;      % Fontsize
lw = 1.5;      % LineWidth
msz = 8;       % MarkerSize

set(gcf,'InvertHardcopy','on');
set(gcf,'PaperUnits', 'inches');
papersize = get(gcf, 'PaperSize');
left = (papersize(1)- width)/2;
bottom = (papersize(2)- height)/2;
myfiguresize = [left, bottom, width, height];
set(gcf,'PaperPosition', myfiguresize);

% Save the file as PNG
print('/home/johanna/latex-svn/study-paper-reports/v5/images/overlapping_bar_all_metrics','-dpng','-r300');

