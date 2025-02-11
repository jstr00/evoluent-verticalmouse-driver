#!/bin/bash
set -e

if [ "$EUID" -ne 0 ]; then
  echo "Please run as root (e.g. sudo ./install.sh)"
  exit 1
fi

# Define installation paths
INSTALL_BIN="/usr/local/bin"
CONFIG_DIR="/etc/mouse_remap"
SERVICE_FILE="/etc/systemd/system/mouse_remap.service"
DESKTOP_FILE="/usr/share/applications/mouse_remap_gui.desktop"
BUILD_DIR="/tmp/mouse_remap_build"

mkdir -p "$BUILD_DIR"

# Compile the remap daemon (no GUI)
echo "Compiling mouse_remap..."
gcc -O2 -Wall -o "$BUILD_DIR/mouse_remap" mouse_remap.c -levdev

# Compile the GUI tool (GTK3 required)
echo "Compiling mouse_remap_gui..."
gcc -O2 -Wall -o "$BUILD_DIR/mouse_remap_gui" mouse_remap_gui.c $(pkg-config --cflags --libs gtk+-3.0)

# Install binaries to /usr/local/bin
echo "Installing binaries..."
install -Dm755 "$BUILD_DIR/mouse_remap" "$INSTALL_BIN/mouse_remap"
install -Dm755 "$BUILD_DIR/mouse_remap_gui" "$INSTALL_BIN/mouse_remap_gui"

# Install configuration file to /etc/mouse_remap (create directory if needed)
echo "Installing configuration file..."
mkdir -p "$CONFIG_DIR"
install -Dm644 config.txt "$CONFIG_DIR/config.txt"

# Create systemd service file to run the mouse_remap daemon continuously.
# The service will use the config file from /etc/mouse_remap/config.txt.
echo "Creating systemd service file..."
cat > "$SERVICE_FILE" <<EOF
[Unit]
Description=Mouse Remap Service
After=usb.target

[Service]
Environment=CONFIG_FILE=$CONFIG_DIR/config.txt
ExecStart=$INSTALL_BIN/mouse_remap -c $CONFIG_DIR/config.txt
Restart=on-failure

[Install]
WantedBy=multi-user.target
EOF

# Reload systemd daemon and enable/start the service.
echo "Enabling and starting service..."
systemctl daemon-reload
systemctl enable mouse_remap.service
systemctl restart mouse_remap.service

# Create a desktop entry for the GUI tool (so the user can launch it from the application menu)
echo "Creating desktop entry..."
cat > "$DESKTOP_FILE" <<EOF
[Desktop Entry]
Version=1.0
Type=Application
Name=Mouse Remap GUI
Comment=Configure the mouse key mappings
Exec=pkexec env DISPLAY=$DISPLAY XAUTHORITY=$XAUTHORITY /usr/local/bin/mouse_remap_gui
Icon=preferences-system
Terminal=false
Categories=Utility;
EOF

# Cleanup
rm -rf "$BUILD_DIR"

echo ""
echo "Installation complete!"
echo "You can run the GUI tool from your application menu or by executing 'sudo mouse_remap_gui' in a terminal."
