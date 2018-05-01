# CellAutoCpp
Cellular Automata Library implemented in C++ <br />
(a course project done in class C++ design, offered by Columbia University Spring 2018)

**Authors:** <br />
Haomin Long <br />
Tongfei Guo <br />
Xiaotian Hu <br />

**Acknowledgements:** <br />
1. to `CellAuto.js` for the idea of this project <br />
2. to Prof. Bjarne Stroustrup for his invaluable instructions. <br />
3. to TAs Kai-Zhan Lee and Abhishek Shah for their suggestions. <br />

**Documentations:**
1. Refer to CellAutoCpp Tutorial.pdf for instructions on how to use this library.
2. Refer to CellAutoCpp Documentation.pdf for detailed description on each of the APIs.
3. Refer to CellAutoCpp Report.pdf for report of this project.

**How to use:**
1. Download all `.cpp` and `.h` file from Source
2. Include header `CellAutoCpp.h` for no visualization version. Compile with `-std=c++14` flag and link with `-lpthread` option.
3. with visualization version.


**Simple Visualization in Console:**

Our library provides a way of visualization in console. Cellular Automata is displayed at certain frequency by mapping cells with specific states to corresponding ASCII characters. To use it, include `CAConsoleViz.h` in your project as well.

In "ConsoleViz_demo" folder, we provide some example codes of visualization, together with a makefile. Use `make release` or `make debug` to compile these files and use `make clean` to erase the generated object files and executables. All executables are stored under the folder of `exe/`.

<img src="https://github.com/Tongfei-Guo/CellAutoCpp/blob/master/Images/consoleviz_fractal.png" width="500" height="784" />


**How to visualize:**
Our library provides visualization. It is implemented with OpenGL version 3.3. To use the visualization, please make sure the glfw3 is deployed as shared library in your computer. To install it, type in:

`sudo apt-get install libglfw3-dev`

In "visualization" folder, we provide some example codes of visualization, together with a makefile. Use `make release` or `make debug` to compile these files and use `make clean` to erase the generated object files and executables. All executables are stored under the folder of `exe/`.

*********NOTE : Please make sure the file for vertex shader instancing.fs and fragment shader instancing.vs exist in the same path of the binary.************* 

**Some of the animated plotting:**
1. Empty Cave &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 2. Cave &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 3.Forest Fire

![emptycave](https://github.com/Tongfei-Guo/CellAutoCpp/blob/master/Images/emptycave.gif)
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
![cave](https://github.com/Tongfei-Guo/CellAutoCpp/blob/master/Images/fullcave.gif)
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
![forestfire](https://github.com/Tongfei-Guo/CellAutoCpp/blob/master/Images/forestfire.gif)

4. Lava &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 5. Splash &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 6.Cyclic

![emptycave](https://github.com/Tongfei-Guo/CellAutoCpp/blob/master/Images/lava.gif)
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
![splash](https://github.com/Tongfei-Guo/CellAutoCpp/blob/master/Images/splash.gif)
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
![cyclic](https://github.com/Tongfei-Guo/CellAutoCpp/blob/master/Images/cyclic.gif)

7. Fractal &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 8. Game Of Life &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 9.Maze

![fractal](https://github.com/Tongfei-Guo/CellAutoCpp/blob/master/Images/fractal.gif)
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
![gameoflife](https://github.com/Tongfei-Guo/CellAutoCpp/blob/master/Images/gameoflife.gif)
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
![maze](https://github.com/Tongfei-Guo/CellAutoCpp/blob/master/Images/maze.gif)

10. Ising(negative M field) &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; 11. Ising(positive M field)  &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;

![negising](https://github.com/Tongfei-Guo/CellAutoCpp/blob/master/Images/negising.gif)
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
![posising](https://github.com/Tongfei-Guo/CellAutoCpp/blob/master/Images/posising.gif)
&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp; &nbsp;
