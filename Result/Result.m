
close all;clc;clear

syc1 = [50, 56, 63, 60, 63, 55, 56, 50, 59, 51, 49, 59, 50, 49, 53, 61, 60, 53, 59, 57, 53, 55, 58, 50];
syc2 = [57, 54, 49, 62, 65, 58, 60, 65, 49, 56, 56, 62, 44, 46, 52, 51, 57];
syc3 = [45, 57, 54, 69, 65, 55, 58, 48, 59, 59, 55, 56, 53, 54, 58, 52, 54, 54, 53, 57, 54, 60, 50];
syc4 = [55, 51, 54, 63, 49, 58, 52, 54, 56, 49, 56, 57, 59, 52, 57, 53, 61, 58, 59, 51, 63, 49, 48, 62, 55];
unsyc1 = [54, 45, 63, 53, 54, 58, 52, 48, 56, 50, 52, 43, 54, 50, 49, 50, 61, 59, 60, 59, 57, 61, 52, 52];
unsyc2 = [53, 56, 57, 61, 58, 50, 54, 54, 49, 43, 50, 65, 53, 65, 66, 57, 55];

syc1_avg = mean(syc1);
syc2_avg = mean(syc2);
syc3_avg = mean(syc3);
syc4_avg = mean(syc4);
unsyc1_avg = mean(unsyc1);
unsyc2_avg = mean(unsyc2);

avg_time = [syc1_avg, syc2_avg, syc3_avg, syc4_avg, unsyc1_avg, unsyc2_avg];

b=bar(avg_time );
grid on
set(gca,'XTickLabel',{'syc1','syc2','syc3','syc4','unsyc1','unsyc2'})
xlabel('Traffic Ligths Strategy');
ylabel('Average Travel Time (s/unit)');

