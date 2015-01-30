


%% use function to test on other data

% pp = imread('color_calibration_red_block/calibration_4.jpg','jpg');
pp = imread('colors/blocks_10','jpg');
pp = pp(1:3:end,1:3:end,:);

bin = 0 * pp(:,:,1);
for i = 1:size(pp,1)
	for j = 1:size(pp,2)
        %h = hsv(i,j,1); s = hsv(i,j,2); v = hsv(i,j,3);
        r = pp(i,j,1); g = pp(i,j,2); b = pp(i,j,3);
        r = double(r); g = double(g); b = double(b);
        bin(i,j) = mask_func(r,g,b,p_r,p_g,p_b,30,10,80);
	end
end

figure;
subplot(121); image(pp); axis equal;
subplot(122); pcolor(flipud(bin)); shading flat; axis equal;
