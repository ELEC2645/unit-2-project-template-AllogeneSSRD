# Physical formula solver

![Teaching](docs/attachments/Teaching%2001_2.gif)

## transistors and optoelectronics

Examples:

1. Cavity mode index by Fabry–Perot: m * (lambda / (2n)) = L

```c
Parsed lambda: 853 nanometer
Parsed n: 3.7 dimensionless
Parsed L: 207 micrometer
Using specified unit for m: 
Fabry–Perot: {'m': '1795.7796 dimensionless'}
```

2. Mode Spacing: Δλ = λ² / (2 n L)

```c
Parsed lambda: 853 nanometer
Parsed n: 3.7 dimensionless
Parsed L: 207 micrometer
Using specified unit for nanometer
Mode Spacing Δλ: {'delta_lambda': '0.4750 nanometer'}
```

3. modes number = bandwidth / Δλ

```c
Parsed lambda: 853 nanometer
Parsed n: 3.7 dimensionless
Parsed L: 207 micrometer
Parsed bandwidth: 6 nanometer
Using specified unit for num_modes: 
modes_number: {'num_modes': '12.6315 dimensionless'}
```

## Power electronics
