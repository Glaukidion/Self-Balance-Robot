clear all;
close all;
clc;
%%Dados do sistema
mh = 0.375; %%[kg] - massa conjunto mecânico
ms = 0.580; %%{kg] - massa motores de passo
mw = 0.056; %%[kg] - massa rodas
g = 9.81;   %%[m/s^2] - aceleração da gravidade
r = 68.4e-3/2;  %%[m] - raio da roda
lh = 77.52e-3;   %%[m]
lu = 48.48e-3;   %%[m]
I0 = 2.792e-3;   %%[kg-m^2] - inercia conjunto mecânico
Im = 1.08e-5;   %%[kg-m^2] - inercia eixo motores de passo
Iw = 2*1.8986e-5; %%[kg-m^2] - inercia das rodas
Ir1 = Iw+Im; %%[kg-m^2] - inercia total conjunto eixos-rodas
Is = 2*8.9933e-5; %%[kg-m^2] - inercia do motor de passo
Ir0 = I0+Is; %%[kg-m^2] - Inercia do motor de passo + haste
Th = 2*16e-2;  %%[N-m] - Torque de holding
Tm = 2*18e-2; %%[N-m] - Torque de detenção
%%fator de conversão rads para graus
k_rad2deg = 180/pi;
%%fator de conversão rads/s para RPM
k_rad2RPM = 9.5493;
Nmax = 14*k_rad2deg*60/360; %%[RPM] - Rotação máxima do motor de passo
Wmax = Nmax*2*pi/60; %%[rad/s] - frequência angular máxima
W2max = Th/Ir1;  %%[rad/s^2] - aceleração angular máxima
Bm = 0.024;
V = 0.0*9.81;  %%[N] - força vertical constante sendo equilibrada
%%Parametros do sistema
Ic = Ir1+mw*r^2;
Id = Ic+mh*r^2;
Jwh = Ic+mh*r^2;
Jh = (1-Is/I0);
k0 = (mh*g*lh+V*(lu+lh))/Ir0;
Jr0 = (1+(Id+mh*lh*r)/Ir0);
a0 = (lu+lh+r)/Ir0;
%%modelo espaço de estados
A = [0 1
    k0/Jr0 0];
B = [0 0
     -Id/(Jr0*Ir0) a0/Jr0];
C = [1 0
     0 1
     k0/Jr0 0];
D = [0 0
    0 0
    -(Id/(Jr0*Ir0)) a0/Jr0];
T = 0:0.001:1;
T0 = 0:0.001:2;
%%Sistema MISO - Saida teta[rads] e entradas dw/dt[rad/s^2] e H(t)[N]
SYS1 = ss(A,B,C,D);
%%Teste da planta: entrada degrau de dw/dt e impulso de H(t)
wp0 = 0.1; %% Aceleração angular [rad/s^2];
ht0 = 0; %%força horizontal [N]
Wp0 = wp0*ones(length(T),1);
Ht0 = zeros(length(T),1);
Ht0(1) = ht0/(T(length(T))/length(T));
U0 = [Wp0 Ht0];
%%Sistema SISO: saida teta[rads] - entrada dw/dt[rad/s^2];
SYS2 = ss(A,B,C(1,:),D(1,:));
%%Plots
figure(1)
%%Plot da planta
Y0 = k_rad2deg.*lsim(SYS1,U0,T);
plot(T,Y0);
legend('teta','theta_1p', 'theta_2p');
%%Dados Experimentais
dt=1e-2;
Teta_15RPM = [
 0
-0.054070797
0.332512212
0.260402306
-0.872330626
-1.145956882
-0.731988256
-0.8868107
-1.812621132
-2.592287858
-2.441772636
-2.507326046
-3.326071421
-4.159861628
-4.360008875
-4.496411005
-5.066768487
-5.965020974
-6.492491881
-6.857463836
-7.345093091
-8.16816616
-8.855191231
-9.528181393
-10.33396085
-11.29002857
-12.18741957
-13.20410792
-14.25389512
-15.35078811
-16.46694247
-17.81242561
-19.25905672
-20.74946085
-22.23175748
-23.97058232
-25.79419772
-27.72946357
-29.78020667
-32.11358706
-34.83022817
-37.72097651
-41.11720911
-45.58661143
-50.66458765
-55.0390536
-59.55548477
];
Time_15RPM = [
0
0.01
0.02
0.03
0.04
0.05
0.06
0.07
0.08
0.09
0.1
0.11
0.12
0.13
0.14
0.15
0.16
0.17
0.18
0.19
0.2
0.21
0.22
0.23
0.24
0.25
0.26
0.27
0.28
0.29
0.3
0.31
0.32
0.33
0.34
0.35
0.36
0.37
0.38
0.39
0.4
0.41
0.42
0.43
0.44
0.45
0.46
];
Teta_30RPM = [
    0
-0.071869993
-0.150035963
0.470179939
-0.468090391
-1.273903027
-1.562461042
-1.84878595
-2.757814089
-3.79988114
-4.425036907
-4.831663449
-5.721007029
-6.7075634
-7.559222221
-8.26369524
-9.068878174
-10.22080549
-11.34346231
-12.34604454
-13.37346808
-14.60974566
-15.88080692
-17.19982243
-18.73707644
-20.49552345
-22.22072728
-23.99789747
-26.03359922
-28.14259148
-30.26292101
-32.69605446
-35.61402512
-38.96777089
-42.97178777
-47.41059875
-52.00529861
-56.70305252
];
Time_30RPM = [
0
0.01
0.02
0.03
0.04
0.05
0.06
0.07
0.08
0.09
0.1
0.11
0.12
0.13
0.14
0.15
0.16
0.17
0.18
0.19
0.2
0.21
0.22
0.23
0.24
0.25
0.26
0.27
0.28
0.29
0.3
0.31
0.32
0.33
0.34
0.35
0.36
0.37
]
%% Correção dos pontos experimentais - retirada de offset
for n = 1:length(Teta_15RPM)
    Teta_15RPM(n) = Teta_15RPM(n)-Teta_15RPM(1);
end
%%
Ts = Time_15RPM(2)-Time_15RPM(1);
Td = 0:0.010:10;
teta_t = fit(Time_15RPM,Teta_15RPM,'exp2');
%%
Uref = 15;
Ue = 30;
s = tf('s');
Ys = ((teta_t.a/(s-teta_t.b))+(teta_t.c/(s-teta_t.d)));
Us =  (Uref*1)/s;
%%função de transferência experimental
Hs_e = Ys/Us;
%%Conversão espaço de estados teórico - função de transferência teórica
[NUM_T,DEN_T] = ss2tf(A,B,C(1,:),D(1,:),1);
%%função de transferência teórica - entrada (rads/s^2); saida(rads)
Hs_tna = tf(NUM_T,DEN_T);
%%transformação-> função de transferência - entrada(RPM); saida(graus)
Hs_tna = 2*k_rad2deg*s*Hs_tna/k_rad2RPM;
%%plot função de transferência teórica(graus) - sem ajustes
    %OBS: O fator 2, advem do fato de o sistema consistir de dois motores
y0 = step(Uref*Hs_tna, Time_15RPM);
%%fator de correlação: curva teórica - experimental
k_TeoExp = 1.11277;
%%Função de transferência teórica ajustada pela experimental
Hs_ta = k_TeoExp*Hs_tna;
%%plots: Experimental x Teórica ajustada
y1=step(Uref*Hs_e,Time_15RPM);
y2=step(Uref*Hs_ta,Time_15RPM);
y3=step(Ue*Hs_ta, Time_15RPM);
figure(3)
hold on;
plot(Time_15RPM,Teta_15RPM,'bo');
plot(Time_30RPM, Teta_30RPM, 'mo');
plot(Time_15RPM,y0,'c-');
plot(Time_15RPM,y1, 'k-');
plot(Time_15RPM, y2(:,1),'rx');
plot(Time_15RPM, y3(:,1),'gx');
legend("Dados experimentais(15RPM)","Dados experimentais(30RPM)","Curva teórica(15RPM) - sem ajuste","Resposta ao degrau(15RPM) - experimental","Resposta ao degrau(15RPM) - Teórico ajustado","Resposta ao degrau(30RPM) - Teórico ajustado");
hold off;
Hz = c2d(Hs_ta,Ts,'zoh');

