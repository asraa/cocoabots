
X_VAL = 12;
Z_VALS = 13:1:(13+23-1);
FILENAME = 'xydata_x12';
I_S = 1:23;
qq = [];

figure; 
for i = I_S
    eval(['pp=imread(''x12_13/blocks_',num2str(i),'.jpg'',''jpg'');']);
    %pp = pp(1:2:end,1:2:end,:);
    imshow(pp)
    shading flat; axis equal
    pbox = ginput;
    ll = pbox(1,:);
    qq = [qq, ll'];
end

pp = [];
xy = [];
for i = 1:size(qq,2)
    pp = [pp, qq(1,i), qq(2,i)];
    xy = [xy, X_VAL, Z_VALS(i)];
end

save(FILENAME,'xy','pp');

%%

xy_meta = [];
pp_meta = [];

load('xydata_x0');
xy_meta = [xy_meta, xy];
pp_meta = [pp_meta, pp];

load('xydata_x6');
xy_meta = [xy_meta, xy];
pp_meta = [pp_meta, pp];

load('xydata_x12');
xy_meta = [xy_meta, xy];
pp_meta = [pp_meta, pp];


%%

beta0 = [685,200,271,0,721,-48,0,0.389,0.92];
beta = nlinfit(xy_meta, pp_meta, @reconstructWmatInv, beta0);
cam_mat = [beta(1), beta(2), beta(3); beta(4), beta(5), beta(6); beta(7), beta(8), beta(9)]

%%
beta0 = [685,271,150,20,4.5];
beta = nlinfit(xy_meta, pp_meta, @reconstructWmatInvBU, beta0);

%%

z = xy_meta(2:2:end); pp = pp_meta(2:2:end);

beta0 = [680,20];
beta = nlinfit(z, pp, @reconstruct2d_inv, beta0)

figure;
scatter(z,pp);
hold on;
plot(z, reconstruct2d_inv(beta,z));
