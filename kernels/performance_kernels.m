clear all
close all
clc
delta = -14:1:21;
projRGB = [32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667   32.6667 32.6667   32.6667   32.6667   32.6667   33.3333   34.0000   38.0000   44.0000   50.6667   53.3333   53.3333   60.0000   59.3333   56.6667 48.6667   39.3333   32.0000   27.3333   23.3333   22.0000   20.6667   46.6667];


plot(delta,projRGB, '-d', 'LineWidth',3,'MarkerSize',3)
%plot(vec_shift,all', '-d', 'LineWidth',3,'MarkerSize',10);