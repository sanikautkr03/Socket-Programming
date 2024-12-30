import socket
import time

def udp_ping_client(host="127.0.0.1", port=12000, num_pings=10, timeout=1):
    # Create a UDP socket
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client_socket.settimeout(timeout)

    # Get the client's own IP address
    client_ip = socket.gethostbyname(socket.gethostname())
    print(f"Client IP address: {client_ip}")

    for sequence_number in range(1, num_pings + 1):
        try:
            # Send "ping" message with sequence number and timestamp
            start_time = time.time()
            message = f"ping {sequence_number} {start_time}"
            client_socket.sendto(message.encode(), (host, port))

            # Wait for "pong" response
            response, server_address = client_socket.recvfrom(1024)
            end_time = time.time()

            # Calculate RTT
            rtt = (end_time - start_time) * 1000  # Convert to milliseconds
            print(f"Ping {sequence_number}: Reply from {server_address[0]}:{server_address[1]}, RTT = {rtt:.2f} ms")
        except socket.timeout:
            print(f"Ping {sequence_number}: Request timed out")

    client_socket.close()

if __name__ == "__main__":
    udp_ping_client()