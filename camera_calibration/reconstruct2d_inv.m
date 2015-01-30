function [result] = reconstruct2d_inv( beta, zz )

%cam_mat = [685 0 295; 0 683 236; 0 0 1];
cam_mat = [beta(1), 0, 295.506; 0, beta(1), 241.44; 0, 0, 1];
%cam_mat = cam_mat * 0.5;

%cam_mat = [345.2863 0 148.7808; 0 344.4604 120.2642; 0 0 1];
theta = beta(2) / 180 * pi;
rot = [1, 0, 0; 0, cos(theta), -sin(theta); 0, sin(theta), cos(theta)];
H = cam_mat * rot

result = [];
for i = 1:length(zz)
    z = zz(i);
    mm = H*[0,11,z]';
    mm = mm / mm(3);
    
    result = [result, mm(2)];
    
%     s_times = H_inv * qq;
%     s = (2-11) / s_times(2); % height of cube
%     
%     coords_3d = s * s_times;
%     
%     result = [result,coords_3d(3)];
end

end

