# Evoluent VerticalMouse D Driver

This project provides a Linux driver for the **Evoluent VerticalMouse D** series. It allows you to remap mouse buttons to different actions and control them using a graphical user interface (GUI) or configuration file. The project was tested on Linux only and does not rely on official driver documentation, but rather on my own tests with the device.

## Features
- Remap mouse buttons (Top, Middle, Wheel, Bottom, Side Top, Side Bottom)
- Save configurations and automatically restart the mouse remap service
- Simple GUI for configuring mouse remapping
- Supports several common key mappings (e.g., Left Click, Right Click, Copy, Paste, etc.)

## Requirements
- **Linux (Tested on Linux Mint)**
- `gcc`
- `gtk+-3.0`
- `libevdev` 

## Installation

### Step 1: Clone the repository
Clone this repository to your local machine:
```bash
git clone https://github.com/jstr00/evoluent-verticalmouse-driver.git
cd evoluent-verticalmouse-driver
```

### Step 2: Install dependencies
Before proceeding, ensure that you have the necessary dependencies installed:
```bash
sudo apt install gcc pkg-config libevdev-dev libgtk-3-dev
```
### Step 3: Install the driver

To install the driver and the graphical interface, use the following command:

```bash
sudo ./install.sh

```

This will:

1. Compile the mouse remap daemon and GUI tool.
2. Install the binaries to /usr/local/bin.
3. Set up the systemd service to run the mouse remap daemon continuously.
4. Create a configuration directory in /etc/mouse_remap/ and install the configuration file.
5. Add a desktop entry for easy access to the GUI tool.

The installation must be done as root, hence `sudo` is required.

## Usage

### GUI Tool
Once the installation is complete, you can run the **Mouse Remap GUI** from the application menu or by typing the following command in the terminal:

```bash
sudo mouse_remap_gui
```

### Command Line
If you prefer not to use the GUI, you can directly edit the configuration file located at /etc/mouse_remap/config.txt. The remapping can be modified by updating the values in this file.

After editing the configuration file, restart the mouse remap service:

```bash
sudo systemctl restart mouse_remap.service
```

## Notes
* The program is designed to be used with sudo for administrative privileges.
* The installation and usage process is intended for Linux systems only.
* No official driver documentation for the Evoluent VerticalMouse D has been inspected. This software is based on my personal testing of the device.

## Contributing
If you would like to request additional button mappings or make improvements to the code, feel free to open an issue or create a pull request.

## Disclaimer
This software does not guarantee any functionality and is provided as-is. No warranties or guarantees are given, and I am not liable for any issues that arise from using this software.