import subprocess
import webbrowser
import time
import os
import sys

ROOT    = os.path.dirname(os.path.abspath(__file__))
VENV_PY = os.path.join(ROOT, 'backend', 'venv', 'bin', 'python')
SERVER  = os.path.join(ROOT, 'backend', 'server.py')

# make sure the venv exists
if not os.path.exists(VENV_PY):
    print("ERROR: venv not found. Run this from the project root and make sure backend/venv exists.")
    sys.exit(1)

print("Starting Flask server...")
server_proc = subprocess.Popen(
    [VENV_PY, SERVER],
    cwd=os.path.join(ROOT, 'backend')
)

print("Waiting for server to start...")
time.sleep(3)

print("Opening dashboard...")
try:
    # works on WSL2 — opens Windows default browser
    subprocess.run(['explorer.exe', 'http://localhost:5000/dashboard'])
except Exception:
    webbrowser.open('http://localhost:5000/dashboard')

print("\nServer is running at http://localhost:5000")
print("Dashboard: http://localhost:5000/dashboard")
print("\nPress Ctrl+C to stop the server.")

try:
    server_proc.wait()
except KeyboardInterrupt:
    print("\nShutting down server...")
    server_proc.terminate()
