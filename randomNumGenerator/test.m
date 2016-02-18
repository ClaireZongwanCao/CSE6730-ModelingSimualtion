clear all; clc; close all;
% Test against the standard uniform
xU = dlmread('randUniform.txt');
nU = length(xU);
nbins =100;
edges = linspace(0,1,11);
expectedCounts = nU * diff(edges);
[hU,pU,stU] = chi2gof(xU,'edges',edges,'expected',expectedCounts);
figure
hist(xU,nbins)

% Test against standard normal (mean 0, standard deviation 1)
xN = dlmread('randNormal.txt');
[hN,pN] = chi2gof(xN,'cdf',@normcdf);
figure
hist(xN,nbins)



% % Test against the Poisson distribution by specifying observed and expected counts
% xP = dlmread('randPoisson.txt');
% bins = 0:99;
% hist(xP,nbins)
% [obsCounts,~] = hist(xP,nbins);
% nP = sum(obsCounts);
% lambdaHat = sum(bins.*obsCounts) / nP;
% expCounts = nP * poisspdf(bins,lambdaHat);
% [hP,pP,stP] = chi2gof(bins,'ctrs',bins,'frequency',obsCounts, ...
%                    'expected',expCounts,'nparams',1)