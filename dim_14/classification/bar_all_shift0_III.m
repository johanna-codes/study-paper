
clear all
close all
clc
%optimal p for Grassmann Manifolds & Removing nor_svm and Overlapping

all = [ 76.0000   68.0000   67.3333   67.3333;
    79.3333   52.6667   50.6667   56.6667    ];


Xlabel = {'One Segment + NN';'One Segment + Dist. Vectors'};

h =  bar(all);
ybuff=2;
for i=1:length(h)
    XDATA=get(get(h(i),'Children'),'XData');
    YDATA=get(get(h(i),'Children'),'YData');
    for j=1:size(XDATA,2)
        x=XDATA(1,j)+(XDATA(3,j)-XDATA(1,j))/2;
        y=YDATA(2,j)+ybuff;
        t=[num2str(YDATA(2,j),3) ,'%'];
        text(x,y,t,'Color','k','HorizontalAlignment','left','Rotation',90)
    end
end

ylim([0 100])
set(gca,'XTickLabel',Xlabel);

l= cell(1,4);
l{1}='LogEucl'; l{2}='SteinDiv'; l{3}='ProjMet'; l{4}='BinCau';
legend(h,l, 'Location', 'BestOutside');



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


%Home
%print('/media/johanna/HD1T/latex-svn/study-paper-report/v3/images/summary_shift0_II','-dpng','-r300');

%Nicta
print('/home/johanna/latex-svn/study-paper-reports/v5/images/summary_shift0_III','-dpng','-r300');