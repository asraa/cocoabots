function [result] = reconstruct2d( beta, qq_zong, actual )

cam_mat = [555.23,  0,    321.96; 
           0,     555.42, 216.43;
           0,       0,     1];
theta = beta / 180 * pi;
rot = [1, 0, 0; 0, cos(theta), -sin(theta); 0, sin(theta), cos(theta)];
H = cam_mat * rot;

H_inv = inv(H);

result = [];
for i = 1:(length(qq_zong)/2)
    qq = [qq_zong((2*i-1):2*i);1];
    s_times = H_inv * qq;
    s = (9-actual) / s_times(2); % height of cube
    coords_3d = s * s_times;
    
    result = [result;coords_3d];
end

end

