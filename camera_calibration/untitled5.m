
qq = [];
figure;
pp = imread('images/blocks_1.jpg','jpg');
% pp = pp(1:2:end,1:2:end,:);
imshow(pp)
shading flat; axis equal
pbox = ginput;
% qq = pbox(1,:);
qq = pbox(:,2);
%%
z = [12:2:40];
save('data_pt_final2','z','qq');

%%
load('data_pt_final2')

beta0 = 10;
beta = nlinfit(z,qq, @reconstruct2d_inv, beta0)

figure; scatter(z, qq);
hold on; plot(z,reconstruct2d_inv(beta,z))

title(num2str(beta));

%%

qq = [];
figure; 
for i = 1:15
    eval(['pp=imread(''images/blocks_',num2str(i),'.jpg'',''jpg'');']);
    %pp = pp(1:2:end,1:2:end,:);
    imshow(pp)
    shading flat; axis equal
    pbox = ginput;
    ll = pbox(1,:);
    qq = [qq, ll'];
end
z = [12:2:40];

save('data_pt_final225','z','qq');

%%

load('data_pt_final225')
pp = [];
for i = 1:size(qq,2)
    pp = [pp, qq(1,i), qq(2,i)];
end
pp = pp';

%z = z(1:13);
%pp = pp(1:26);

beta0 = 10;
beta = nlinfit(z,pp(2:2:end), @reconstruct2d_inv, beta0)

figure; scatter(z, pp(2:2:end));
hold on; plot(z,reconstruct2d_inv(beta,z))

title(num2str(beta));

%%






%%
hold on;
for beta = -20:5:20
    scatter(z, reconstruct2d_inv(beta,z));
end
%%

cam_mat = [685*0.5 0 295*0.5; 0 683*0.5 236*0.5; 0 0 1];
theta = beta / 180 * pi;
rot = [1, 0, 0; 0, cos(theta), -sin(theta); 0, sin(theta), cos(theta)];
H = cam_mat * rot
H_inv = inv(H)
