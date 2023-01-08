clc,clear
    images = dir('C:\Users\ELONSUN\Desktop\acsdSaliency\acsdSaliency\image\*.jpg');
    num_images = length( images );
    for i = 1 : num_images  
       image_file = fullfile( 'C:\Users\ELONSUN\Desktop\acsdSaliency\acsdSaliency\image\', images(i).name ); 
       image  = imread( image_file);  
       image1  = imresize(image,[328 328]);  
       imwrite( image1 , image_file , 'jpg' );  
     end   