import socket

def udp_ping_server(host="127.0.0.1", port=12000):
    # Create a UDP socket
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    server_socket.bind((host, port))
    print(f"Server listening on {host}:{port}")

    while True:
        try:
            # Receive message and client address
            message, client_address = server_socket.recvfrom(1024)
            print(f"Received from {client_address[0]}:{client_address[1]}: {message.decode()}")

            # Send "pong" response
            server_socket.sendto(b"pong", client_address)
        except KeyboardInterrupt:
            print("\nServer shutting down.")
            break

    server_socket.close()

if __name__ == "__main__":
    udp_ping_server()