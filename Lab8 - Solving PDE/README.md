# Math Lab 8. Partial Differential Equations
### Task

The task is to solve transfer equations using 3 schemes:
* central
* rectangle
* Lax-Wendroff

Transfer equations to solve:
* du/dx + du/dt = 0
* du/dx + du/dt = 2 \* (x + t)

### Results
#### General

The plots show graphs of F(x, t) at a fixed time. <br>
Steps: x = 0.01, t = 0.001.

#### First equation 

du/dx + du/dt = 0<br>
u(x, 0) = x<br>
u(0, t) = -t<br>
Analytical(x, t) = x - t<br>

![](./assets/seq-(x-t)-central.svg)
![](./assets/seq-(x-t)-lax.svg)
![](./assets/seq-(x-t)-rectangle.svg)
![](./assets/seq-(x-t)-analytical.svg)

#### Second equation

du/dx + du/dt = 2\*(x + t)<br>
u(x, 0) = x\*x<br>
u(0, t) = t\*t<br>
Analytical(x, t) = x\*x + t\*t<br>

![](./assets/seq-(x*x+t*t)-central.svg)
![](./assets/seq-(x*x+t*t)-lax.svg)
![](./assets/seq-(x*x+t*t)-rectangle.svg)
![](./assets/seq-(x*x+t*t)-analytical.svg)

### Conclusion
As we can see from plots, all schemes converge to the analytical solution if (t_step / x_step) < 1.<br>
The maximum deviation turned out to be <0.5 with used steps.
<br>

Rectangle scheme cannot be properly parallelized since it requires node from the same t-line.<br>
Lax and central schemes shows performance boost:
* 1 thread:  1.9s 
* 2 threads: 1.2s
* 4 threads: 0.8s


## Prerequisites

- C++20 capable compiler
- MPI package
- gnuplot utility


## Installation
- Clone the repository:
```sh
git clone https://github.com/NikitaDzer/computational-math.git
```
- Compile the project:
```sh
cd computational-math/Lab8\ -\ Solving\ PDE/
cmake -S . -B build
make -C build -j4
```

## Running
- Run sequantial tests:
```sh
./build/lab8
```

- Run parallel tests (4 threads):
```sh
mpirun -n 4 ./build/lab8 1
```

---
♥ WITH LOVE
