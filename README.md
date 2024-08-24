# Image Processing Project

This project is a simple console-based Image Processing application developed in C++ during my first semester. It operates on PGM (Portable GrayMap) format files and includes a variety of basic image processing functions.

## Table of Contents
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Contributing](#contributing)

## Features
The application provides the following image processing features:
1. **Load Image**: Load a PGM image file.
2. **Save Image**: Save the processed image to a PGM file.
3. **Change Brightness**: Adjust the brightness of the image.
4. **Linear Contrast Stretching**: Enhance the contrast of the image.
5. **Adjust Sharpness**: Modify the sharpness of the image.
6. **Convert to Binary**: Convert the image to a binary format.
7. **Image Resize By Certain Ratio**: Resize the image by a given ratio.
8. **Image Rotation**: Rotate the image by a specified angle.
9. **Flip an Image**: Flip the image horizontally or vertically.
10. **Crop an Image**: Crop a specified portion of the image.
11. **Join two Images**: Combine two images into one.
12. **Apply Mean or Median Filter**: Apply mean or median filters for noise reduction.
13. **Apply General Linear Filter From File**: Apply a custom filter using coefficients from a file.
14. **Enhance an Image by Applying Filter**: Improve image quality by applying a predefined filter.
15. **Derivative Mask**: Apply a derivative mask to highlight edges.
16. **Find Edges by Derivative**: Detect edges using a derivative-based method.
17. **Translate an Image**: Shift the image by a certain number of pixels.
18. **Scale an Image**: Scale the image dimensions.
19. **Rotate an Image**: Another option to rotate the image.
20. **Flip an Image**: Another option to flip the image.
21. **Exit**: Exit the application.

## Installation
To compile and run the project, follow these steps:

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/your-repo-name.git
    ```
2. Navigate to the project directory:
    ```bash
    cd your-repo-name
    ```
3. Compile the project using a C++ compiler:
    ```bash
    g++ main.cpp -o ImageProcessing
    ```

## Usage
Once the application is running, you can choose from the menu to perform various image processing tasks. The menu is intuitive and guides you through each option.

## Project Structure
- `main.cpp`: The main file containing the application logic.
- `filters.cpp`: Contains functions for applying filters.
- `MainMenu.txt` : The File to Load Main Menu.
- `First.pgm` : An Image File to test it.
- `README.md`: Project documentation.

## Contributing
Contributions are welcome! If you have ideas for new features or improvements, feel free to fork the repository and submit a pull request.

