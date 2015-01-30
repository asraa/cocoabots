function [ coords_3d ] = reconstructWmat( beta, cam_coord, actual )

cam_mat = [beta(1), beta(2), beta(3); beta(4), beta(5), beta(6); beta(7), beta(8), beta(9)];
H = cam_mat
H_inv = inv(H)

qq = [cam_coord';1];
s_times = H_inv * qq;
s = (11-actual) / s_times(2) % height of cube
coords_3d = s * s_times;


end

