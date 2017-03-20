# Color Manipulation of digital images using openCV

## Part A
- Display continuous changes in color for the xy and the Luv representations. 

#### Program 1
- The input to the program is a width and a height.
- The output is two images of dimensions width × height that are displayed on the screen.
- For the xy image, the pixel at row i and column j should have the color value:
  
  x = j/width, y = i/height, Y = 1
- For the Luv image, the pixel at row i and column j should have the color value: 
  
  L = 90, u = 512 ∗ j/width − 255, v = 512 ∗ i/height − 255

## Part B
- Change the color of the image based on a histogram computed from a window in the image.
- The window is specified in terms of the normalized coordinates w1 h1 w2 h2, where the window upper left point is (w1,h1), and its lower right point is (w2,h2). 
#### Program 2
- Input is a color image, performs linear scaling in the Luv domain, and writes the scaled image as output. 
- The scaling in Luv should stretch only the luminance values. You are asked to apply linear scaling that would map the smallest L value in the specified window and all values below it to 0, and the largest L value in the specified window and all values above it to 100.
#### Program 3
- Input is a color image, performs histogram equalization in the Luv domain, and writes the scaled image as output. Histogram equalization in Luv is applied to the luminance values, as computed in the specified window. It requires a discretization step, where the real-valued L is discretized into 101 values.
- As in the second program, all L values below the smallest L value in the window should be mapped to 0, and all L value above the largest L value in the window should be mapped to 100.
#### Program 4
- The scaling is to be performed in the xyY domain. 
- The scaling should stretch only the luminance (Y) values. You are asked to apply linear scaling that would map the smallest Y value in the specified window and all values below it to 0, and the largest Y value in the specified window and all values above it to 1.
