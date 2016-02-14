NAME: Amruta Kulkarni
USC ID: 6914-9708-27
E-mail: arkulkar@usc.edu

The following steps will guide you to execute the files that are contained in this folder.

FOR PROBLEM_1A TO PROBLEM_2D
1.Copy this entire folder on your Desktop.
2.In each folder containing a main_filename.cpp file, copy the image, which you want to give as an input, to that particular file 
3.Now open Terminal
4.Give the path of main_filename.cpp file for each problem that you want to run.
5.For example consider problem_1A.  
Now if you want to run main_swirlKate.cpp,
then in terminal type the following after the $ sign:
cd ./Desktop/EE569_hw1_6914970827_Kulkarni/Problem_1A
Then type the following:
make
The code will ask you to enter your files.Please do so accordingly with the help of the example that gets displayed in terminal.
6.So after you reach the main_filename.cpp and type make, you also need to give the required file names as will be shown in an example when you type make.
7.Now, the output files that get generated after the execution of this code will get stored in the same folder as that of your current main_filename.cpp.
8.You can then view these files,which are raw images, by using ImageJ software.
9.Repeat these steps for Problem_1A to problem_2D

FOR PROBLEM_3A
For Part_1
1. Create a new project in Xcode and copy the contents of main_1.cpp in the main.cpp of the new project.
2. Set the custom directory path in the scheme of Xcode to this new project folder, as per where it is located on your computer.
3. Copy the Horseshoe.raw image in this folder.
4. Now, run the main.cpp file of your project
5. The output image will get generated in this same folder and you can view it using ImageJ.

Repeat the same for Part_2, but instead of main_1.cpp, just copy main_2.cpp.

FOR PROBLEM_3B

FOR PART_1
1. Create a new project in Xcode and copy the contents of main_holefilling.cpp in the main.cpp of the new project.
2. Set the custom directory path in the scheme of Xcode to this new project folder, as per where it is located on your computer.
3. Copy the Horse1.raw image in this folder.
4. Now, run the main.cpp file of your project
5. 3 output images will get generated in the same folder as the main.cpp file.
6. You can view these images by using ImageJ.

FOR PART_2
1. Create a new project in Xcode and copy the contents of main_thinning.cpp in the main.cpp of the new project.
2. Set the custom directory path in the scheme of Xcode to this new project folder, as per where it is located on your computer.
3. Copy the hole-filledHorse.raw image that was generated in PART_1 above, to this new project folder
4. Now, run the main.cpp file of your project
5. 2 output images will get generated in the same folder as the main.cpp file.
6. You can view these images by using ImageJ.

FOR PART_3
Repeat the same steps as above, but instead of main_thinning.cpp, just copy main_skeletonizing.cpp.