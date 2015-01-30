function [ result ] = reconstructWmatInvBU(beta, glob_coords)

cam_mat = [beta(1), 0, beta(2); 0, beta(1),  beta(3); 0, 0, 1];
theta_x = beta(4)/180*pi;
rot_x = [1, 0, 0; 0, cos(theta_x), -sin(theta_x); 0, sin(theta_x), cos(theta_x)];
H = cam_mat * rot_x

result = [];
for i = 1:(length(glob_coords)/2)
    
    glob_x = glob_coords(2*i-1);
    glob_z = glob_coords(2*i);
    
    vec = [glob_x-beta(5), 11, glob_z]'; % on the floor
    
    mm = H*vec;
    mm = mm' / mm(3); % row vector
    
    result = [result, mm(1:2)];
    
end

end

