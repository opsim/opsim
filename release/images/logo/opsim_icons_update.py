#!/usr/bin/env python3

# This script updates opsim bitmap files from the SVG files
import os
import subprocess
import sys
import shutil

def run(cmd):
    print("   ", " ".join(cmd))
    subprocess.check_call(cmd)

# set the inkscape executable path
inkscape_bin = shutil.which("inkscape")

if sys.platform == 'darwin':
    inkscape_app_path = '/Applications/Inkscape.app/Contents/Resources/script'
    if os.path.exists(inkscape_app_path):
        inkscape_bin = inkscape_app_path

#inkscape not found from path, try to add a default one
if inkscape_bin == None:
    if sys.platform == 'win32':
        inkscape_bin = 'C:\Program Files\Inkscape\inkscape.exe'


BASEDIR = os.path.abspath(os.path.dirname(__file__))

cmd = (
    inkscape_bin,
    os.path.join(BASEDIR, "OpSim.svg"),
    "--export-width=16",
    "--export-height=16",
    "--without-gui",
    "--export-png=" + os.path.join(BASEDIR, "opsim16.png"),
)
run(cmd)

cmd = (
    inkscape_bin,
    os.path.join(BASEDIR, "OpSim.svg"),
    "--export-width=32",
    "--export-height=32",
    "--without-gui",
    "--export-png=" + os.path.join(BASEDIR, "opsim32.png"),
)
run(cmd)

cmd = (
    inkscape_bin,
    os.path.join(BASEDIR, "OpSim.svg"),
    "--export-width=48",
    "--export-height=48",
    "--without-gui",
    "--export-png=" + os.path.join(BASEDIR, "opsim48.png"),
)
run(cmd)

cmd = (
    inkscape_bin,
    os.path.join(BASEDIR, "OpSim.svg"),
    "--export-width=64",
    "--export-height=64",
    "--without-gui",
    "--export-png=" + os.path.join(BASEDIR, "opsim64.png"),
)
run(cmd)

cmd = (
    inkscape_bin,
    os.path.join(BASEDIR, "OpSim.svg"),
    "--export-width=128",
    "--export-height=128",
    "--without-gui",
    "--export-png=" + os.path.join(BASEDIR, "opsim128.png"),
)
run(cmd)

cmd = (
    inkscape_bin,
    os.path.join(BASEDIR, "OpSim.svg"),
    "--export-width=256",
    "--export-height=256",
    "--without-gui",
    "--export-png=" + os.path.join(BASEDIR, "opsim256.png"),
)
run(cmd)


cmd = (
    inkscape_bin,
    os.path.join(BASEDIR, "logo.svg"),
    "--export-width=4000",
    "--without-gui",
    "--export-png=" + os.path.join(BASEDIR, "logo.png"),
)
run(cmd)
