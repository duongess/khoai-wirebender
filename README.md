# Khoai Wirebender

Software system and firmware for a 2-axis (Feed & Bend) CNC wire bending machine. This project encompasses a desktop drawing application (Python) and real-time processing firmware designed for microcontrollers (C++).

## 1. Directory Structure

* `include/`: Contains `Config.h` to configure Arduino pin assignments, mechanical constants, and error tolerances.
* `master/`: The Python application used to draw CNC paths and auto-generate C++ header files.
* `lib/PathPlanner/`: Algorithm to interpolate drawn paths into an array of vectors.
* `lib/Kinematics/`: Resolves kinematics, converting vectors (length/angle) into actual mechanical servo strokes.
* `lib/CommandQueue/`: A Ring Buffer implementation designed to prevent RAM overflow during execution.
* `lib/HardwareController/`: Handles PWM signals and direct Servo control.
* `src/main.cpp`: The main orchestrator managing data flow from PathPlanner -> Kinematics -> Queue -> Hardware.

## 2. System Requirements

* **Python 3.8+** with the `poetry` package manager installed.
* **Visual Studio Code** with the `PlatformIO` extension.
* **Native C++ Compiler** (GCC/Clang) for running PC simulations.

## 3. Getting Started

### Step 1: Draw and Generate CNC Data (Python Environment)
Navigate to the `master` directory, install the required dependencies, and launch the drawing board.

```bash
# Install virtual environment and dependencies
cd master
poetry install

# Launch the drawing interface
poetry run python planner.py

```

*Note: After finishing your drawing, click "Done" on the UI. The system will automatically generate the `PathData.h` file and push it directly into the C++ library directory.*

### Step 2: Run PC Simulation (Native Environment)

This allows you to verify the analytical math and the command queue logic directly on your computer without needing physical hardware.

```bash
# Return to the root directory of the project
cd ..

# Clear the build cache, then compile and execute on the PC
pio run -e native -t clean
pio run -e native -t exec

```

### Step 3: Upload Firmware to Microcontroller (Arduino Environment)

Connect your Arduino board to the PC via a USB cable. The system will compile the code and flash the data into the microcontroller's memory.

```bash
# Clear the build cache to prevent hardware conflicts
pio run -e uno -t clean

# Compile and upload the firmware to the Arduino
pio run -e uno -t upload

```

## 4. Hardware Notes

**CRITICAL:** Please double-check the `include/Config.h` file to ensure you have correctly wired the PWM signal pins for both the **Feed Servo** and **Bend Servo** before providing power to the system.