function [result] = reconstruct2d( beta, qq_zong, actual )

%cam_mat = [685 0 295; 0 683 236; 0 0 1];
cam_mat = [682.62, 0, 295.506; 0, 682.241, 241.44; 0, 0, 1];
%cam_mat = [345.2863 0 148.7808; 0 344.4604 120.2642; 0 0 1];
theta = beta / 180 * pi;
rot = [1, 0, 0; 0, cos(theta), -sin(theta); 0, sin(theta), cos(theta)];
H = cam_mat * rot;

H =[  709.7257  123.7006  268.3691;0  745.6183  -77.8274;0    0.4186    0.9082];
H = [ 702.2781   94.2056  255.0148;  0  723.1313  -69.1183;  0    0.3465    0.9380];
H_inv = inv(H)

result = [];
for i = 1:(length(qq_zong)/2)
    qq = [qq_zong((2*i-1):2*i);1];
    s_times = H_inv * qq;
    s = (11-actual) / s_times(2); % height of cube
    coords_3d = s * s_times;
    
    result = [result;coords_3d];
end

end

