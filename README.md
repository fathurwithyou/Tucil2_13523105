# Image Compressor (Quadtree-based)
![C++](https://img.shields.io/badge/C++-17-blue?logo=c%2B%2B&logoColor=white)

Image Compressor is a program that compresses images using the **Quadtree** structure and the **Divide and Conquer** approach. This method recursively divides the image into quadrants based on variance thresholds, reducing storage while preserving important details.

## Features
- Compresses images using **Quadtree decomposition**.
- Supports **multiple error measurement methods** (Variance, MAD, Max Pixel Difference, Entropy, and SSIM for bonus).
- Allows **user-defined parameters**: threshold, minimum block size, and target compression percentage.
- Outputs **compressed image and GIF visualization** of the Quadtree formation.

## Project Structure
```
ImageCompressor/
├── bin/
│   ├── ImageCompressor.o
│   ├── main
│   ├── main.o
│   ├── Metrics.o
│   ├── Quadtree.o
│   ├── QuadtreeNode.o
├── doc/
├── include/
│   ├── Color.hpp
│   ├── ImageCompressor.hpp
│   ├── Metrics.hpp
│   ├── Quadtree.hpp
│   ├── QuadtreeNode.hpp
├── LICENSE
├── main.cpp
├── Makefile
├── README.md
├── src/
│   ├── ImageCompressor.cpp
│   ├── Metrics.cpp
│   ├── Quadtree.cpp
│   ├── QuadtreeNode.cpp
└── test/
```

## Installation
### Linux
Ensure you have **g++ (C++17 or higher)** and install the required library:
```bash
sudo apt install g++ libfreeimage-dev
```
### Windows
Download and install **MinGW-w64** and **FreeImage** library manually, **but using WSL is higly recommended**.

## Compilation
Run the following command to build and run the project:
```bash
make
```
To clean compiled files:
```bash
make clean
```

## Usage
The program will prompt for user inputs:
1. **Input Image Path** - Absolute path of the image to be compressed.
2. **Error Calculation Method** - (1: Variance, 2: MAD, 3: Max Pixel Difference, 4: Entropy, 5: SSIM *[Bonus]*).
3. **Threshold** - Determines block division.
4. **Minimum Block Size** - Defines the smallest allowed block size.
5. **Target Compression Percentage** - Set between 0 (disabled) and 1.0 (100% compression).
6. **Output Image Path** - Absolute path to save the compressed image.
7. **Output GIF Path** (Bonus) - Path to store the visualization.

## Output
- **Compressed Image**: Saved at the specified output path.
- **GIF Visualization** *(Optional)*: Shows step-by-step Quadtree formation.
- **Console Output**: Displays execution time, image sizes, compression percentage, and Quadtree statistics.

## Example Input Format
```
test/image.jpg
2
0.05
4
0.8
test/compressed.jpg
test/compression.gif
```

## Author
| Name | NIM | Class |
|------|------|------|
| Muhammad Fathur Rizky | 13523105 | K02 |

---
This project was developed for IF2211 **Strategi Algoritma - Tugas Kecil 2** at Institut Teknologi Bandung.

