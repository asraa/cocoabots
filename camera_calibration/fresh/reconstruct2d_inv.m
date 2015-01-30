function [result] = reconstruct2d_inv( beta, zz )

cam_mat = [555.23,  0,    321.96; 
           0,     555.42, 216.43;
           0,       0,     1];
theta = beta / 180 * pi;
rot = [1, 0, 0; 0, cos(theta), -sin(theta); 0, sin(theta), cos(theta)];
H = cam_mat * rot;

result = [];
for i = 1:length(zz)
    z = zz(i);
    mm = H*[0,9,z]';
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

