function result = mask_func( r,g,b,p_r,p_g,p_b,tol_r,tol_g,tol_b )
%UNTITLED3 Summary of this function goes here
%   Detailed explanation goes here


R_WEIGHT = 0.2126;
G_WEIGHT = 0.7152;
B_WEIGHT = 0.0722;

intensity = R_WEIGHT*r + G_WEIGHT*g + B_WEIGHT*b;

if((b<230 && abs(polyval(p_r,intensity)-r) < tol_b) || ...
        (abs(r - 255) < 20)&& ...
        abs(polyval(p_r,intensity)-r) < tol_r && ...
        abs(polyval(p_b,intensity)-b) < tol_b)
    result = 1;
else 
    result = 0;

end

