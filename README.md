# Prime Number Calculator — Serial vs Parallel

A C program that counts and sums prime numbers within a given range using either **serial** (single-process) or **parallel** (multi-process via `fork()`) execution. Built for CS 3305 — Operating Systems at Western University.

## Table of Contents

- [Overview](#overview)
- [How It Works](#how-it-works)
- [Getting Started](#getting-started)
- [Usage](#usage)
- [Example Output](#example-output)
- [Performance Comparison](#performance-comparison)
- [Technologies](#technologies)

## Overview

This program accepts a numeric range and computes all prime numbers within it. The range is divided into **four equal quartiles**, each processed independently. Two execution modes are supported:

- **Serial (mode 0):** All four quartiles are processed sequentially by the parent process.
- **Parallel (mode 1):** Four child processes are spawned using `fork()`, each handling one quartile concurrently.

Each quartile reports its **PID**, **parent PID**, **prime count**, and **prime sum**.

## How It Works

1. The input range `[min, max)` is split into four quartiles.
2. Depending on the mode:
   - **Serial:** The parent iterates through each quartile one at a time.
   - **Parallel:** The parent forks four child processes. Each child computes primes for its assigned quartile and exits. The parent waits for all children to finish.
3. For each quartile, a trial-division primality test determines which numbers are prime.

## Getting Started

### Prerequisites

- GCC (or any C99-compatible compiler)
- A Unix/Linux environment (uses `fork()`, `wait()`, `getpid()`, `getppid()`)

### Compilation

```bash
gcc -o assignment-1 assignment-1.c
```

## Usage

```
./assignment-1 <mode> <min> <max>
```

| Argument | Description                                      |
|----------|--------------------------------------------------|
| `mode`   | `0` for serial, any other integer for parallel   |
| `min`    | Start of range (inclusive)                        |
| `max`    | End of range (exclusive, must be greater than min)|

### Examples

```bash
# Serial mode, range 0–100
./assignment-1 0 0 100

# Parallel mode, range 0–100000
./assignment-1 1 0 100000
```

## Example Output

**Serial (0–100):**

```
Process id: 1440374
pid: 1440374, ppid 14377448 - Count and sum of prime numbers between 0 and 25 are 9 and 100
pid: 1440374, ppid 14377448 - Count and sum of prime numbers between 25 and 50 are 6 and 228
pid: 1440374, ppid 14377448 - Count and sum of prime numbers between 50 and 75 are 6 and 384
pid: 1440374, ppid 14377448 - Count and sum of prime numbers between 75 and 100 are 4 and 348
```

**Parallel (0–100):**

```
Process id: 1440379
pid: 1440380, ppid 1440379 - Count and sum of prime numbers between 0 and 25 are 9 and 100
pid: 1440381, ppid 1440379 - Count and sum of prime numbers between 25 and 50 are 6 and 228
pid: 1440382, ppid 1440379 - Count and sum of prime numbers between 50 and 75 are 6 and 384
pid: 1440383, ppid 1440379 - Count and sum of prime numbers between 75 and 100 are 4 and 348
```

> Notice how in serial mode every line shares the same PID (the parent), while in parallel mode each quartile has a unique child PID with the parent's PID as its PPID.

## Performance Comparison

Timed with the `time` command on a university Linux server.

| Range       | Mode     | Real Time | User Time | Sys Time |
|-------------|----------|-----------|-----------|----------|
| 0 – 100     | Serial   | 0.002s    | 0.001s    | 0.000s   |
| 0 – 100     | Parallel | 0.002s    | 0.002s    | 0.000s   |
| 0 – 100,000 | Serial   | 0.741s    | 0.741s    | 0.000s   |
| 0 – 100,000 | Parallel | 0.316s    | 0.742s    | 0.000s   |

For small ranges the overhead of forking negates any benefit, but for larger ranges the parallel version achieves a significant wall-clock speedup (~2.3× for 0–100,000) by distributing work across multiple CPU cores.

## Technologies

- **Language:** C (C99)
- **System Calls:** `fork()`, `wait()`, `getpid()`, `getppid()`, `exit()`
- **Platform:** Linux (tested on Western University's `compute` server)

## Author

Melvin Roger — University of Western Ontario, CS 3305

## License

This project was completed as a course assignment. Please reference responsibly.
