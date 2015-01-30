function bin = mask_func_mult( rgb_meta )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

N = size(rgb_meta, 3) / 3;

R_WEIGHT = 0.2126;
G_WEIGHT = 0.7152;
B_WEIGHT = 0.0722;

rgb_meta = double(rgb_meta);

bin = zeros(size(rgb_meta, 1),size(rgb_meta, 2)*N);
for n = 1:N
    n
    for i = 1:size(rgb_meta, 1)
        for j = 1:size(rgb_meta, 2)
            r = rgb_meta(i,j,3*n-2);
            g = rgb_meta(i,j,3*n-1);
            b = rgb_meta(i,j,3*n);
            intensity = R_WEIGHT*r + G_WEIGHT*g + B_WEIGHT*b;            
            if r > 1.2*g && r > 1.2*b
                bin(i,size(rgb_meta,2)*(n-1)+j) = 1;
            end
        end
    end
end

