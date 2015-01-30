
qq1 = [];
qq2 = [];

qw = -10:1:10;

for ll = qw
    
    theta = ll/180*pi;
    
    cam_mat = [685 0 295; 0 683 236; 0 0 1];
    
    rot = [1, 0, 0; 0, cos(theta), -sin(theta); 0, sin(theta), cos(theta)];
    
    H = cam_mat * rot;
    H_inv = inv(H);
    
    X = [0,11-2,14]';
    Y = [30,2-11,17.5]';
    
    im_coord1 = cam_mat * rot * X;
    im_coord2 = cam_mat * rot * Y;
    
    mm = im_coord1 / im_coord1(3);
    nn = im_coord2 / im_coord2(3);
    
    qq1 = [qq1, im_coord1(2)/im_coord1(3)];
    qq2 = [qq2, im_coord2(2)/im_coord2(3)];
end

%%

figure;
plot(qw, qq2, qw, qq1);
