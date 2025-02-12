function plotStuff()

global path

global x Sa edge augs 

numothers = 2;
numtracks = 8;
numplots = numtracks+numothers;

mapLimits = [];


%% load stuff

posef = sprintf('%s%s',path, 'PoseSE3(W).log');
x = sortByT(stubbornLoad(posef));

%% -------------------- FIGURA 1

%% plot trajectory

set(0,'CurrentFigure',1)
clf
hold on

i = find(x(:,2) == max(x(:,2)));

plot3(x(i,3),x(i,4),x(i,5),'m');
plot3(x(1:i(1),3),x(1:i(1),4),x(1:i(1),5),'k');

for j = i(1:20:length(i))'    
%for j = [size(x,1):-1:1 1]
    plotAxis(x(j,3:5)',x(j,6:9),1)    
end    

grid on
%axis manual
axis equal
%axis([-4 4 -4 4])

%% -------------------- FIGURA 2

set(0,'CurrentFigure',2)
clf

%% Accelerometer
edgename = 'IMUintegralDeltaP';

edgef = sprintf('%s%s.log',path, edgename);

if exist(edgef, 'file')
    
    edge = sortByT(stubbornLoad(edgef));
    oi = find(edge(:,1) >= x(i(1),1),1);
    
    axis tight    
    %% plot error
    subplot(2,numplots,1)
    
    %plot(edge(oi:end,1),edge(oi:end,25:27))    
    plot(edge(:,1) - edge(1,1),edge(:,50:52))
    title({'IMU $\varepsilon$ $\Delta P$' ''}, 'Interpreter','latex')
    set(gca,'FontSize', 7)

    axis tight

    %% plot measure
    subplot(2,numplots,numplots +1)

    %plot(edge(oi:end,1),[edge(oi:end,22:24)])
    plot(edge(:,1) - edge(1,1),[edge(:,23:25)])
    
    title({'IMU z  $\Delta P$' ''}, 'Interpreter','latex') 
    set(gca,'FontSize', 7)
    
    axis tight
end

%% Gyroscope
edgename = 'IMUintegralDeltaQ';

edgef = sprintf('%s%s.log',path, edgename);

if exist(edgef, 'file')
    
    edge = sortByT(stubbornLoad(edgef));
    oi = find(edge(:,1) >= x(i(1),1),1);
    
    axis tight    
    %% plot error
    subplot(2,numplots,2)
    
    %plot(edge(oi:end,1),edge(oi:end,25:27))    
    plot(edge(:,1) - edge(1,1),edge(:,39:41))
    title({'IMU $\varepsilon$  $\Delta Q$' ''}, 'Interpreter','latex')
    set(gca,'FontSize', 7)

    axis tight

    %% plot measure
    subplot(2,numplots,numplots + 2)

    %plot(edge(oi:end,1),[edge(oi:end,22:24)])
    plot(edge(:,1) - edge(1,1),[edge(:,23:26)])
    
    title({'IMU z  $\Delta Q$' ''}, 'Interpreter','latex') 
    set(gca,'FontSize', 7)
    
    axis tight
end


%% -------------------- LANDMARK

for M = 0:numtracks

    edgename = sprintf('Track_%d', M);

    edgef = sprintf('%s%s.log',path, edgename);

    if exist(edgef, 'file')

        edge = sortByT(stubbornLoad(edgef));
        oi = find(edge(:,1) >= x(i(1),1),1);

        if M < numtracks
        %% plot error
        subplot(2,numplots,numothers + M + 1)

        plot(edge(:,1) - edge(1,1),edge(:,[25:26])) %xy img
        title({strcat('\verb|',edgename,'| $\varepsilon$') ''}, 'Interpreter','latex')
        set(gca,'FontSize', 7)
        
        axis tight
        
        %% plot measure
        subplot(2,numplots,numplots + numothers + M + 1)
        
        plot(edge(:,1) - edge(1,1),edge(:,[23:24])) % xy z
        title({strcat('\verb|',edgename,'| z') ''}, 'Interpreter','latex')
        set(gca,'FontSize', 7)

        axis tight
        end
        %% plot landmarks on map
        
        set(0,'CurrentFigure',1)

%         %ImagePlaneProjection  
%         
%         Lf = sprintf('%s%s%s.log',path, edgename,'_Lw');
%         L = sortByT(stubbornLoad(Lf));
% 
%         hold on
%         plot(L(1,3), L(1,4), 'ro')
%         text(L(1,3), L(1,4), sprintf('L%d', M));

        %FramedHomogeneousPoint
        
        HPf = sprintf('%s%s%s.log',path, edgename,'_HP');
        HP = sortByT(stubbornLoad(HPf));
        
        ai = find(x(:,1) == HP(1,1),1);
        A = x(ai,:);

        hold on
        plot3(A(1,3), A(1,4), A(1,5), 'ro')
        text(A(1,3), A(1,4), sprintf('AP%d', M));
        
        % compute 3d point
        LW = A(3:5)'+1/HP(5)*quatrot(A(6:9))*[HP(3) HP(4) 1]';

        plot3(LW(1), LW(2), LW(3), 'ro')
        text(LW(1), LW(2), LW(3), sprintf('L%d', M));

        
        set(0,'CurrentFigure',2)
        
    end
end