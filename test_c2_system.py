import threading
import subprocess
import time
import requests
import json

APP_URL = "http://127.0.0.1:5000"
ROUTE_PATH = "shell"  # Fixed route path

def start_app():
    # Start the Flask app (app.py)
    subprocess.Popen(["python3", "app.py"])

def start_client():
    # Start the client agent (client_agent.py)
    subprocess.Popen(["python3", "client_agent.py"])

def wait_for_server():
    # Wait for the server to be up
    for _ in range(30):
        try:
            r = requests.get(f"{APP_URL}/{ROUTE_PATH}")
            if r.status_code == 200:
                return True
        except:
            time.sleep(1)
    return False

def send_command(command, target_ip):
    url = f"{APP_URL}/{ROUTE_PATH}"
    headers = {'Content-Type': 'application/json'}
    data = {'command': command, 'target_ip': target_ip}
    r = requests.post(url, headers=headers, data=json.dumps(data))
    return r.json()

def main():
    print("Starting Flask app server...")
    app_thread = threading.Thread(target=start_app)
    app_thread.daemon = True
    app_thread.start()

    time.sleep(5)  # Wait for server to start

    print("Starting client agent...")
    client_thread = threading.Thread(target=start_client)
    client_thread.daemon = True
    client_thread.start()

    time.sleep(10)  # Wait for client to connect

    target_ip = "127.0.0.1"  # Assuming client connects from localhost

    print(f"Sending 'pwd' command to {target_ip}...")
    response = send_command("pwd", target_ip)
    print("Response:", response)

    print(f"Sending 'cd ..' command to {target_ip}...")
    response = send_command("cd ..", target_ip)
    print("Response:", response)

    print(f"Sending 'pwd' command again to {target_ip}...")
    response = send_command("pwd", target_ip)
    print("Response:", response)

    # Test file upload
    print(f"Testing file upload to {target_ip}...")
    # Create a test file to upload
    test_file_content = "This is a test file for upload."
    with open("test_upload.txt", "w") as f:
        f.write(test_file_content)
    response = send_command("upload test_upload.txt /tmp/test_upload.txt", target_ip)
    print("Upload Response:", response)

    # Test file download
    print(f"Testing file download from {target_ip}...")
    response = send_command("download /tmp/test_upload.txt test_download.txt", target_ip)
    print("Download Response:", response)

    # Verify downloaded file content
    try:
        with open("test_download.txt", "r") as f:
            content = f.read()
        print("Downloaded file content:", content)
    except Exception as e:
        print("Error reading downloaded file:", e)

    print("Test script completed.")

if __name__ == "__main__":
    main()
