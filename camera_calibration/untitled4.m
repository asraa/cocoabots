

% load('data_pts');

qq = [];
for i = 1
    eval(['pp=imread(''images/blocks_',num2str(i),'.jpg'',''jpg'');']);
    figure;
    imshow(pp)
    shading flat; axis equal
    pbox = ginput;
    ll = pbox(1,:);
    qq = [qq, ll'];
end

qq = [qq; ones(1,size(qq,2))];

%%

actual = 8.4 - 14;

qw = -20:1:20;
zz = [];

for ll = qw
    
    theta = ll/180*pi;
    
    cam_mat = [685 0 295; 0 683 236; 0 0 1];
    
    rot = [1, 0, 0; 0, cos(theta), -sin(theta); 0, sin(theta), cos(theta)];
    
    H = cam_mat * rot;
    H_inv = inv(H);
    
    ww = H_inv * qq;    
    
    s = actual / ww(2);
    
    rr = s * ww;
    zz = [zz,rr(3)];
 
end

figure; plot(qw,zz);