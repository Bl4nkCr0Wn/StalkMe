import Globals
import socket

def get_history(soc):
	soc.send('get history\n')
	
	receiving_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	print receiving_socket
	receiving_socket.bind((Globals.target, Globals.HISTORY_TRANSFER_PORT))
	receiving_socket.listen(1)
	client_soc, addr = receiving_socket.accept()
	print addr
	data = client_soc.recv(Globals.TCP_BUFFER_SIZE)
	
	file_size = int(data[:data.find('\n')])
	whole_file = data[data.find('\n') + 1:]
	received_bytes = len(whole_file)
	
	while file_size > received_bytes:
		data = client_soc.recv(Globals.TCP_BUFFER_SIZE)
		received_bytes += len(data)
		whole_file += data
	
	receiving_socket.close()
	client_soc.close()
	with open('History.db', 'wb') as f:
		f.write(whole_file)
		