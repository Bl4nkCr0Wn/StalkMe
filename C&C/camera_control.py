import socket
import numpy
import cv2
import Globals

def live(soc):
	soc.send('camera=on\n')
	#t = threading.Thread(target=camera.watch_feed)
	#t.start()
	#t.join()
	watch_feed()
	print soc.send('camera=off\n')


def watch_feed():
	# Create another socket - UDP for live feed
	udp_soc = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
	udp_soc.bind((Globals.target,Globals.LIVEFEED_PORT))
	
	FRAME_SIZE, addr = udp_soc.recvfrom(4096)
	print addr, FRAME_SIZE
	FRAME_SIZE = int(FRAME_SIZE)
	
	while True:
		# Capture frame-by-frame
		frame = ''
		while len(frame) < FRAME_SIZE:
			frame += udp_soc.recv(Globals.UDP_BUFFER_SIZE)#cap.read()
			#print frame
		
		# Display the resulting frame
		frame = numpy.fromstring(frame, dtype='uint8')
		frame = numpy.reshape(frame[:1280*720], (720, 1280))
		#frame = cv2.imdecode(frame,cv2.IMREAD_COLOR )
		frame_mat = cv2.UMat(frame)
		cv2.imshow('LIVE FEED', frame_mat)

		# Press Q on keyboard to  exit
		if cv2.waitKey(25) & 0xFF == ord('q'):
			break
			# Break the loop
	cv2.destroyAllWindows()
	udp_soc.close()
