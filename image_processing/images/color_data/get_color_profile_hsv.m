function [intensity_vals, h_vals, s_vals, v_vals] = get_color_profile_hsv(rgb, mask)

hsv = rgb2hsv(rgb);

% weights for intensity calculation
R_WEIGHT = 0.2126;
G_WEIGHT = 0.7152;
B_WEIGHT = 0.0722;

% initiate arrays
intensity_vals = [];
h_vals = [];
s_vals = [];
v_vals = [];
rgb = rgb2hsv(rgb);
for i = 1:size(rgb,1)
	for j = 1:size(rgb,2)
        if mask(i,j)
            r = rgb(i,j,1); g = rgb(i,j,2); b = rgb(i,j,3);  
            h = hsv(i,j,1); s = hsv(i,j,2); v = hsv(i,j,3);
			v = R_WEIGHT*r + G_WEIGHT*g + B_WEIGHT*b;
			intensity_vals = [intensity_vals, v];
			h_vals = [h_vals, h];
			s_vals = [s_vals, s];
			v_vals = [v_vals, v];	            
        end		
	end
end

intensity_vals = double(intensity_vals);
h_vals = double(h_vals);
s_vals = double(s_vals);
v_vals = double(v_vals);

h_vals = h_vals * 255;
s_vals = s_vals * 255;
v_vals = v_vals * 255;

end

