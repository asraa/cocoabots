
% crop image range of interest
[rgb_raw, map] = imread('../red.jpg','jpg');
figure; image(rgb_raw); 
box = ginput

%%

% use box values to determine rows and cols to crop
rgb_raw = imread('red.jpg');
rgb = rgb_raw(220:460,190:425,:);

% convert to hsv to filter out color 'by eye'
hsv = rgb2hsv(rgb);

% weights for intensity calculation
R_WEIGHT = 0.2126;
G_WEIGHT = 0.7152;
B_WEIGHT = 0.0722;

% initiate arrays
intensity_vals = [];
r_vals = [];
g_vals = [];
b_vals = [];
bin = 0 * rgb(:,:,1);
for i = 1:size(rgb,1)
	for j = 1:size(rgb,2)
        h = hsv(i,j,1); s = hsv(i,j,2); v = hsv(i,j,3);
        % manually tweak to find pixels to include for computation
%         bin(i,j) = (h<120/255 && h>40/255 && s>0.24);
        bin(i,j) = (h<20/255 && h>0/255);
        if bin(i,j)==1
			r = rgb(i,j,1); g = rgb(i,j,2); b = rgb(i,j,3);            
			v = R_WEIGHT*r + G_WEIGHT*g + B_WEIGHT*b;
			intensity_vals = [intensity_vals, v];
			r_vals = [r_vals, r];
			g_vals = [g_vals, g];
			b_vals = [b_vals, b];	            
        end		
	end
end

[intensity_vals,I] = sort(intensity_vals);
r_vals = r_vals(I);
g_vals = g_vals(I);
b_vals = b_vals(I);

figure;
subplot(131); image(rgb_raw); axis equal;
subplot(132); image(rgb); axis equal;
subplot(133); pcolor(flipud(bin)); shading flat; axis equal;

figure; hold on;
scatter(intensity_vals,r_vals,'r');
scatter(intensity_vals,g_vals,'g');
scatter(intensity_vals,b_vals,'b');

clearvars -except intensity_vals r_vals g_vals b_vals

%%
a = [0,255,0,260];
figure; 
subplot(131); scatter(intensity_vals(1:10:end),r_vals(1:10:end),'r'); axis(a);
subplot(132); scatter(intensity_vals(1:10:end),g_vals(1:10:end),'g'); axis(a);
subplot(133); scatter(intensity_vals(1:10:end),b_vals(1:10:end),'b'); axis(a);

