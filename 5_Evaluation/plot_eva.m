clc 
clear all;
dataset = 'LFSD';
A = load(['C:\Users\ELONSUN\Desktop\script\5_Evaluation\tools\Results\',dataset,'\DeepSaliency.mat']);
B = load(['C:\Users\ELONSUN\Desktop\script\5_Evaluation\tools\Results\',dataset,'\ACSD.mat']);
C = load(['C:\Users\ELONSUN\Desktop\script\5_Evaluation\tools\Results\',dataset,'\Ours.mat']);
%D是显著图经过osth的二值化处理
D = load(['C:\Users\ELONSUN\Desktop\script\5_Evaluation\tools\Results\',dataset,'\Ours2.mat']);
%%画PR曲线
figure(1)
pre_DS = A.Pre;
rec_DS = A.Recall;
pre_ACSD = B.Pre;
rec_ACSD = B.Recall;
pre_Mine = C.Pre;
rec_Mine = C.Recall;
plot(pre_DS,rec_DS,'r','LineWidth',2);
hold on;
plot(pre_ACSD,rec_ACSD,'b','LineWidth',2);
hold on;
plot(pre_Mine,rec_Mine,'g','LineWidth',2);
legend('Deep Saliency','ACSD','Ours');
xlabel('Recall');
ylabel('Precision');
title('Precision-recall (PR)');
grid on;
saveas(gcf, [dataset,'_PR'], 'png')
%%画ROC曲线
figure(2)
FPR_DS = A.FPR;
TPR_DS = A.TPR;
FPR_ACSD = B.FPR;
TPR_ACSD = B.TPR;
FPR_Mine = C.FPR;
TPR_Mine = C.TPR;
plot(FPR_DS,TPR_DS,'r','LineWidth',2);
hold on;
plot(FPR_ACSD,TPR_ACSD,'b','LineWidth',2);
hold on;
plot(FPR_Mine,TPR_Mine,'g','LineWidth',2);
hold on;
x = 0:0.1:1;
y = 0:0.1:1;
plot(x,y,'k--');
legend('Deep Saliency','ACSD','Ours');
xlabel('False Positive Rate (FPR)');
ylabel('True Positive Rate (TPR)');
title('Receiver operating characteristics (ROC)');
grid on;
saveas(gcf, [dataset,'_ROC'], 'png')
%%画AUC,FMeasure,MAE柱形图
figure(3)
x = categorical({'AUC','F-Measure','MAE'});
AUC = [A.AUC B.AUC C.AUC];
FMeasure = [A.MaxFmeasure B.MaxFmeasure C.MaxFmeasure];
MAE = [A.MAE B.MAE D.MAE];
y = [AUC;FMeasure;MAE];
bar(x,y);
legend('DeepSaliency','ACSD','Ours');
saveas(gcf, [dataset,'_OTHERS'], 'png')


