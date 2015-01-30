
qq = [];
mm = -30:2:30;

for ll = mm
    cam_mat = [685 0 295; 0 683 236; 0 0 1];
    cam_mat_inv = inv(cam_mat);
    theta = ll/180*pi;
    % need to check rotation direction / how the coordinate system is set up
    rot = [1, 0, 0; 0, cos(theta), -sin(theta); 0, sin(theta), cos(theta)];
    
    H = cam_mat*rot;
    H = cam_mat;
    H_inv = inv(H);
    
    p1 = [0,222,1]';
    p2 = [0,138,1]';
    rot2 = rot(2:3,2:3);
    actual = [2,0]';
    
    h2 = H_inv(2,:);
    h3 = H_inv(3,:);
    MEGA = [h2*p1,-h2*p2;h3*p1,-h3*p2];
    
    S = MEGA \ rot2*actual;
    
    oo = H_inv*p2*S(2);
    qq = [qq,oo(3)];
    
end

figure; plot(mm,qq)

%%

P = [p1,p2];
P = P(2:3,:)

uu = H(2:3,2:3) * actual

P \ uu
