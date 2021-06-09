import socket

ip="192.168.1.200"
port=502

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((ip, port))

adu = struct.pack('>HHHBBHH', 
	0, 0, # TID/ PID
	6, 3, # Length / Unit ID 
	3, 30001, 2) # Function Code / Data (Reg addr, Count)

sock.write(adu)

rsp = sock.recv(512)
