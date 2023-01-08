clc;
clear all;
jpgs = dir('..\Dataset\LFSD\Image\*.jpg');  
num_jpgs = length( jpgs );  
    for i = 1 : num_jpgs 
    Org_image=imread(['..\Dataset\LFSD\Image\',jpgs(i).name]);
    Org_image = double(Org_image);
    
    Dep_Sal=imread(['..\SaliencyMap\LFSD\ACSD\',jpgs(i).name]);
    H = fspecial('disk');
    Dep_Sal = imfilter(Dep_Sal,H);    % blur Depth Saliency Map
    Dep_Sal = rescale(Dep_Sal,0,1); 
    
    hsv = rgb2hsv(Org_image); %颜色空间转换 
    H = hsv(:,:,1); % 色调 
    S = hsv(:,:,2); % 饱和度 
    V = hsv(:,:,3); % 亮度
    V = rescale(V,0,255);
    V = Dep_Sal.*V;
    hsv(:,:,3) = 1.5 * V;
    Aft_pix_product = hsv2rgb(hsv);  
    Aft_pix_product = uint8(Aft_pix_product);
    Org_image = uint8(Org_image);
    figure(2)
    subplot(1,3,1), imshow(Org_image);
    subplot(1,3,2), imshow(Dep_Sal);
    subplot(1,3,3), imshow(Aft_pix_product);
    imwrite(Aft_pix_product,['..\Dataset\LFSD\Processed-image\',jpgs(i).name]);
    end