
load('red_blocks/red_block_meta');
%load('green_blocks/green_block_meta');
%load('blue_line/blue_line_meta');
%load('purple_line/purple_line_meta');
%load('yellow_line_meta');


%%

fn = 'yellow_line_meta';
%fn = 'blue_line_meta';
N = 2;

rgb0 = maskedRGBImage;
mask0 = BW;
rgb_meta = rgb0;
mask_meta = mask0;

for n = 1:N
    eval(['rgb','=maskedRGBImage',num2str(n),';']);
    eval(['mask','=BW',num2str(n),';']);
    rgb_meta = cat(3,rgb_meta,rgb);
    mask_meta = cat(3,mask_meta,mask);
end

save(fn, 'rgb_meta', 'mask_meta');

clearvars -except rgb_meta mask_meta


%% run from here
rgb_meta = double(rgb_meta);
[intensity_vals, r_vals, g_vals, b_vals] = get_color_profile(rgb_meta, mask_meta);


figure; hold on;
scatter(intensity_vals,r_vals,'r');
scatter(intensity_vals,g_vals,'g');
scatter(intensity_vals,1.5*b_vals,'b');
%%
deg = 1;
p_r = polyfit(intensity_vals, r_vals, deg);
p_g = polyfit(intensity_vals, g_vals, deg);
p_b = polyfit(intensity_vals, b_vals, deg);
hold on;
p_r = [1,0];
a = 0:3:200;
plot(a,polyval(p_r,a),'k');
plot(a,polyval(p_g,a),'k');
plot(a,polyval(p_b,a),'k');

%%
rgb_meta = imread('messy_2.jpg','jpg');
%%
rgb_meta = double(rgb_meta);
mask_test = mask_func_mult(rgb_meta,1);
figure; imshow(mask_test); shading flat;

%%
%rgb_meta = double(rgb_meta);
mask_test = mask_func_mult_hsv(rgb_meta,0);
figure; imshow(mask_test); shading flat;







%%

deg = 1;
to_plot = 1;
N = 4;

% weights for intensity calculation
R_WEIGHT = 0.2126;
G_WEIGHT = 0.7152;
B_WEIGHT = 0.0722;

% initiate arrays
intensity_vals = [];
r_vals = [];
g_vals = [];
b_vals = [];
for n = 0:N
rgb = rgb_meta(:,:,(3*n+1):(3*n+3));
rgb = rgb2hsv(rgb);
mask = mask_meta(:,:,n+1);
for i = 1:size(rgb,1)
	for j = 1:size(rgb,2)
        if mask(i,j)
            r = rgb(i,j,1); g = rgb(i,j,2); b = rgb(i,j,3);            
			v = R_WEIGHT*r + G_WEIGHT*g + B_WEIGHT*b;
			intensity_vals = [intensity_vals, v];
			r_vals = [r_vals, r];
			g_vals = [g_vals, g];
			b_vals = [b_vals, b];	            
        end		
	end
end
end

intensity_vals = double(intensity_vals);
r_vals = double(r_vals);
g_vals = double(g_vals);
b_vals = double(b_vals);

%[intensity_vals,I] = sort(intensity_vals);
%r_vals = r_vals(I);
%g_vals = g_vals(I);
%b_vals = b_vals(I);

clearvars -except intensity_vals r_vals g_vals b_vals mask_meta rgb_meta deg to_plot

p_r = polyfit(intensity_vals, r_vals, deg);
p_g = polyfit(intensity_vals, g_vals, deg);
p_b = polyfit(intensity_vals, b_vals, deg);

if to_plot == 1
figure; hold on;
scatter(intensity_vals,r_vals,'r');
scatter(intensity_vals,g_vals,'g');
scatter(intensity_vals,b_vals,'b');
%a = [0:0.01:1];
%plot(a,polyval(p_r,a),'k');
%plot(a,polyval(p_g,a),'k');
%plot(a,polyval(p_b,a),'k');
end

clearvars -except intensity_vals r_vals g_vals b_vals mask_meta rgb_meta p_r p_g p_b





%%

rgb_meta = cat(3,rgb0,rgb1,rgb2);
mask_meta = cat(3,mask0,mask1,mask2);

save('red_block_meta', 'rgb_meta', 'mask_meta');

%%
beta0 = [1,0,230];
beta_red = nlinfit(intensity_vals, double(r_vals), @line_fit_cutoff, beta0);
beta_green = nlinfit(intensity_vals, double(g_vals), @line_fit_cutoff, beta0);
beta_blue = nlinfit(intensity_vals, double(b_vals), @line_fit_cutoff, beta0);


%%
a = [0,255,0,260];
figure; 
subplot(131); scatter(intensity_vals(1:10:end),r_vals(1:10:end),'r'); axis(a);
subplot(132); scatter(intensity_vals(1:10:end),g_vals(1:10:end),'g'); axis(a);
subplot(133); scatter(intensity_vals(1:10:end),b_vals(1:10:end),'b'); axis(a);

