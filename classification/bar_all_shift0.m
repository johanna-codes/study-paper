
clear all
close all
clc


all = [ 80.6667   69.3333   76.0000   76.0000;
    76.0000   68.0000   54.6667   54.6667;
    79.3333   68.0000   32.6667   32.6667;
    52.0000   53.3333   32.6667   32.6667];

Xlabel = {'Overlap';'One Segment';'SVM';'nor_SVM'};

h =  bar(all);
set(gca,'XTickLabel',Xlabel);

l= cell(1,4);
l{1}='LogEucl'; l{2}='SteinDiv'; l{3}='ProjMet'; l{4}='BinCau';
legend(h,l);



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



%Nicta
%print('/home/johanna/latex-svn/latex_rioux/reports/study-paper/images/summary_shift0','-dpng','-r300');