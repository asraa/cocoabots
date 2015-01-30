function [intensity_vals, r_vals, g_vals, b_vals] = get_color_profile(rgb, mask)

% weights for intensity calculation
R_WEIGHT = 0.2126;
G_WEIGHT = 0.7152;
B_WEIGHT = 0.0722;

% initiate arrays
intensity_vals = [];
r_vals = [];
g_vals = [];
b_vals = [];
%rgb = rgb2hsv(rgb);
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

intensity_vals = double(intensity_vals);
r_vals = double(r_vals);
g_vals = double(g_vals);
b_vals = double(b_vals);

end

