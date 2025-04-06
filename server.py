import socket

# Create a UDP socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
server_socket.bind(("127.0.0.1", 12345))

print("Server is listening...")

while True:
    data, addr = server_socket.recvfrom(1024)  # buffer size
    # print(f"Received from {addr}: {data.decode()}")
    print(f"{data.decode()}")
