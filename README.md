# DC-DC Converter Calculator

![Static Badge](https://img.shields.io/badge/language-C-blue)
![GitHub License](https://img.shields.io/github/license/ELEC2645/unit-2-project-template-AllogeneSSRD)

![GitHub commit activity](https://img.shields.io/github/commit-activity/t/ELEC2645/unit-2-project-template-AllogeneSSRD)
![GitHub last commit](https://img.shields.io/github/last-commit/ELEC2645/unit-2-project-template-AllogeneSSRD)

A interactive C command-line tool designed for Power Electronics engineers and students. It assists in calculating parameters for **Buck** and **Boost** converters in both **CCM** (Continuous Conduction Mode) and **DCM** (Discontinuous Conduction Mode).

## Installation & Build

<details open>
<summary>Install</summary>

1. Clone the repository
```bash
git clone https://github.com/ELEC2645/unit-2-project-template-AllogeneSSRD

# Navigate to the cloned directory
cd unit-2-project-template-AllogeneSSRD
```
2. Compile the program

```bash
# Windows
gcc main.c funcs.c -o main.exe -lm

# Linux (Github Codespaces)
gcc main.c funcs.c -o main.out -lm

# the -lm at the end links the math library
```

</details>

<details open>
<summary>Run</summary>

```bash
# Windows
.\main.exe

# Linux (Github Codespaces)
./main.out
```

</details>


## Usage Examples

### 1. Main Menu
Main menu will show upon launching, select your converter topology and operating mode:

```bash
----------- Main menu -----------

        1. Buck Converter - CCM
        2. Buck Converter - DCM
        3. Boost Converter - CCM
        4. Boost Converter - DCM
        5. Exit

-----------------------------------
```


### 2. Calculation
The tool allows you to calculate any variable in a formula by entering `?`.

**Example:** Calculating Input Voltage ($V_i$) for a Buck Converter.

```bash
>> Buck Converter - CCM Mode
Formula (Duty cycle): K = Vo / Vi
Variables: Vo (Output Voltage), Vi (Input Voltage), K (Duty Cycle)
Provide any two values to calculate the other.
Enter '?' for the unknown variable.

Please enter value of Output Voltage (Vo: V): 15
Please enter value of Input Voltage (Vi: V): ?
Please enter value of Duty Cycle (K): 0.2

Calculated Input Voltage (Vi = Vo / K): 75.0000 V
```


![Teaching](docs/attachments/Teaching%2001_2.gif)


## 📄 License

This project is licensed under the [Apache License](LICENSE)


Copyright © 2025 [University of Leeds](https://www.leeds.ac.uk/)
Licensed under the Apache License, Version 2.0
