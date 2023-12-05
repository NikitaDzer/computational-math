# Math Lab 5. Numerical integration
### Task
The task is to numerically integrate a tabulated function and a fast oscillating function using the [trapezoid method](https://en.wikipedia.org/wiki/Trapezoidal_rule).

### Results
#### Interpolated f(x) and f(x)sin(kx)
![Plot for the cubic spline.](./assets/spline.svg)

### Conclusion
As we can see from the plot, to calculate the integral of the fast oscillating function, a small step (<0.01) is needed. <br>
The values of the integrals:
* [VII 9.5a](https://mipt.ru/education/chair/computational_mathematics/study/materials/compmath/other/Aristova_Zavyalova_Lobanov_2014.pdf):  I = 1.60314, step = 0.25
* [VII 9.13d](https://mipt.ru/education/chair/computational_mathematics/study/materials/compmath/other/Aristova_Zavyalova_Lobanov_2014.pdf): I = 7.3074e-10, step = 1.0e-10


## Prerequisites

- C++20 capable compiler
- gnuplot utility


## Installation
- Clone the repository:
```sh
git clone https://github.com/NikitaDzer/computational-math.git
```
- Compile the project:
```sh
cd computational-math/Lab5\ -\ Numerical\ integration/
cmake -S . -B build
make -C build -j4
```

## Running
- Generate plots and print calculated integrals:
```sh
./build/lab5
```
---
♥ WITH LOVE
