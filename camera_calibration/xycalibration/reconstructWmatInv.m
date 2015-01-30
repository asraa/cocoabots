function [ result ] = reconstructWmatInv(beta, glob_coords)

cam_mat = [beta(1), beta(2), beta(3); beta(4), beta(5), beta(6); beta(7), beta(8), beta(9)];
H = cam_mat;

result = [];
for i = 1:(length(glob_coords)/2)
    
    glob_x = glob_coords(2*i-1);
    glob_z = glob_coords(2*i);
    
    vec = [glob_x, 11, glob_z]'; % on the floor
    
    mm = H*vec;
    mm = mm' / mm(3); % row vector
    
    result = [result, mm(1:2)];
    
end

end

