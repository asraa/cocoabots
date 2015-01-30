function bin = mask_func_mult( rgb_meta, color )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here

N = size(rgb_meta, 3) / 3;

R_WEIGHT = 0.2126;
G_WEIGHT = 0.7152;
B_WEIGHT = 0.0722;

rgb_meta = double(rgb_meta);

bin = zeros(size(rgb_meta, 1),size(rgb_meta, 2)*N);
for n = 1:N
    for i = 1:size(rgb_meta, 1)
        for j = 1:size(rgb_meta, 2)
            r = rgb_meta(i,j,3*n-2);
            g = rgb_meta(i,j,3*n-1);
            b = rgb_meta(i,j,3*n);
            intensity = R_WEIGHT*r + G_WEIGHT*g + B_WEIGHT*b; 
            if color == 1 && r > 1.2*g && r > 1.2*b && (r > 1.4*intensity+10 || intensity > 230 && r > 250) ...
                && g < 0.9*intensity+10 && b < 0.9*intensity+10
                bin(i,size(rgb_meta,2)*(n-1)+j) = 1;
            elseif color == 2 && ...
                        abs(g-1.08*intensity-1.57) < 50 && g > r && g > b
                     bin(i,size(rgb_meta,2)*(n-1)+j) = 1;   
            elseif color == 3 && ...
                    r>b && g > b
                bin(i,size(rgb_meta,2)*(n-1)+j) = 1;
            end
        end
    end
end

