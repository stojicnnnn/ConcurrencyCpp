# Organ Transplant Waiting List - Thread-Safe Implementation

A thread-safe C++ implementation of an organ transplant waiting list system that manages patients waiting for transplants and tracks treated patients.

## Overview

This project implements a concurrent data structure that maintains two lists:
- Patients waiting for organ transplants
- Patients who have received transplants (with treatment dates)

The implementation ensures thread safety, allowing multiple threads to safely perform operations without data corruption.

## Features

### Base Assignment
- Add patients to waiting list
- Move patients from waiting to treated list with treatment date
- Delete treated patients before a specified date
- List all waiting patients
- List all treated patients
- Query patient status (waiting/treated/not found)
- Thread-safe using mutex locks

### Bonus 1: Move Semantics
- Move constructor for efficient ownership transfer
- Move assignment operator

### Bonus 2: Enhanced Concurrency
- Uses `std::shared_mutex` for improved read performance
- Multiple threads can read simultaneously
- Write operations remain exclusive
- Optimized for read-heavy workloads

## Building the Project
```bash
# Create build directory
mkdir build
cd build

# Configure with CMake
cmake ..

# Build
make
```

## Running Tests

### Standard test execution
```bash
./src/assignment1_test
```

### With Thread Sanitizer (recommended)
```bash
setarch `uname -m` -R ./src/assignment1_test
```

The thread sanitizer detects data races and ensures thread safety. ASLR (Address Space Layout Randomization) must be disabled using `setarch` for the sanitizer to work properly.

## Project Structure
```
organ_transplant_waiting_list/
├── CMakeLists.txt
├── src/
│   ├── CMakeLists.txt
│   ├── organ_transplant.h          # Header file with class declarations
│   ├── organ_transplant.cpp        # Implementation
│   └── organ_transplant_test.cpp   # Unit tests using Google Test
```

## Requirements

- C++17 or later
- CMake 3.14 or later
- GCC with thread sanitizer support
- Linux environment (for setarch command)

## Thread Safety

All operations are protected using mutex locks:
- Write operations (add, treat, delete) use exclusive locks
- Read operations use shared locks (Bonus 2) allowing concurrent reads
- Move operations properly handle mutex ownership transfer

## Testing

The project includes comprehensive tests covering:
- Basic functionality (add, treat, delete, query)
- Thread safety with concurrent operations
- Move semantics
- Stress tests with multiple threads
- Data race detection using thread sanitizer