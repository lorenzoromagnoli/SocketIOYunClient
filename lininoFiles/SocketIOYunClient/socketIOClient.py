#!/usr/bin/env python
from socket import socket, AF_INET, SOCK_STREAM, SOL_SOCKET, SO_REUSEADDR, gethostname
import sys
import logging
import threading
from websocket import create_connection
from socketIO_client import SocketIO, BaseNamespace
import simplejson as json
import code

class SERIAL:

	class MSG:
		ADDR 			= chr(29)
		VAR				= chr(30)
		DATA			= chr(31)
		END 			= chr(7)
		CONFIRM 		= chr(3)

	class CONNECTION:
		START 			= chr(28)
		END 			= chr(27)
		ERROR 			= chr(26)

class OPT:
	SERVER 			= '--server'
	PORT 			= '--port'
	NAME 			= '-n'
	DESCRIPTION 	= '-d'
	SUBSCRIBER   	= '-s'
	PUBLISHER 		= '-p'

class ERROR:
    OK                  = (0,   '')
    SERVER_MISSING     	= (201, '(' + OPT.SERVER + ') server host not specified')
    SERVER_INVALID     	= (202, '(' + OPT.SERVER + ') "%s" is not a valid host name')
    PORT_MISSING     	= (203, '(' + OPT.PORT + ') port number not specified')
    PORT_INVALID    	= (204, '(' + OPT.PORT + ') "%s" is not a valid port number.')
    NAME_MISSING     	= (205, '(' + OPT.NAME + ') name is required and was not specified.')
    DESCRIPTION_MISSING = (206, '(' + OPT.DESCRIPTION + ') description not provided.')
    SUBSCRIBER_MISSING 	= (207, '(' + OPT.SUBSCRIBER + ') subscriber only accepts two args: type and name. %i args provided.')
    PUBLISHER_MISING 	= (208, '(' + OPT.PUBLISHER + ') publisher only accepts two args: type and name.  %i args provided.')



"""
#this class is used to log code
class Tee(object):
  def __init__(self, log_fname, mode='a'):
    self.log = open(log_fname, mode)

  def __del__(self):
    # Restore sin, so, se
    sys.stdout = sys.__stdout__
    sys.stdir = sys.__stdin__
    sys.stderr = sys.__stderr__
    self.log.close()

  def write(self, data):
    self.log.write(data)
    self.log.flush()
    sys.__stdout__.write(data)
    sys.__stdout__.flush()

  def readline(self):
    s = sys.__stdin__.readline()
    sys.__stdin__.flush()
    self.log.write(s)
    self.log.flush()
    return s

sys.stdout = sys.stderr = sys.stdin = Tee('consolelog.dat', 'w')


connected=0
#logging.basicConfig(level=logging.DEBUG)

#get the host address and the port from the parameter
total = len(sys.argv)
host=str(sys.argv[1])
port=int(sys.argv[2])

socketIO = SocketIO(host, port)

#prepare a new function to be executed when a cmd is receive from the server
def on_cmd(*args):
    print 'received new cmd', args
    start()


#the function connect will be called in a new thread so that we can use this console for commands while the connection run in the background with waiting for connection untill stopped. 
def connectForever(h,p):
    print ("connecting")
    print ("host address: %s" % host)
    print ("port: %s" % port)
    socketIO.on('cmd', on_cmd)
    socketIO.wait()    
    

def start():
    command = raw_input('--> ')
    commandarray = command.split(" ")
    
    if commandarray[0] == "connect":
        global connected
        if( connected==0):
            t = threading.Thread(target=connectForever, args = (host,port))
            t.daemon = True
            t.start()
            print("connected")
            command = raw_input('--> ')
    
    if commandarray[0] == "send":
        totalArgs=len(commandarray)
        #print("n arguments",totalArgs)
        
        message=commandarray[1]
        args=dict()
        for i in range(2,totalArgs-1,2):
            args[commandarray[i]]= commandarray[i+1]
        
        jsonArgs=json.dumps(args)
        #print(jsonArgs)
        socketIO.emit(message,args)


    
while True : # this will loop until invalid_input is set to be True
    start()  
"""
class Namespace(BaseNamespace):
	
	def on_open(self):
		print '[opened]'
		
	def on_connect(self):
		print '[Connected]'
		logging.debug("connection confirmed by server")
		SIO.connected=True
		if SIO._console: 
			SIO._console.log(SERIAL.CONNECTION.START)
			logging.debug("connection started")
	
	def on_error(self,ws,error):
		print '[Error]'
 		if SIO._console: 
 			error_msg = SERIAL.CONNECTION.ERROR + str(error) + SERIAL.MSG.END
	 		SIO._console.log( error_msg )

	def on_close(self, ws):
		print '[Close]'
		if SIO.connected:
			SIO.connected = False
	 		if SIO._console: 
		 		SIO._console.log( SERIAL.CONNECTION.END )

		while SIO.started and not self.connected:
			time.sleep(5)
			SIO.run()
	
	def on_msg(self, *args):
		print 'on_msg', args
		print '[MSG RECEIVED]'
		logging.debug("received a new Ciao from the server")
		
		jsonString=json.dumps(args)
        
		print(jsonString)
		jsonObj=json.loads(jsonString)
		
		var = 'Ciao'
		data = jsonObj[0].get(var)
		logging.debug("response "+var+" "+data)
		
		
		if SIO._console: 
			SIO._console.write("msg",var, data)
			logging.debug("sending msg to Arduino")
			

class socketIOYUNClient(object):   
	
	 
	def __init__(self, server="54.84.192.169", port=""):
		self.server = server
		self.port = port
		self.connected = False
		self.started = False
		self.sio = None
		self._console = {}
		
		
	def console(self, _console = {}):
		self._console = _console

	def start(self):
		self.sio = SocketIO(self.server,self.port,Namespace)
		
		#define callback function for what a certain event is happening
		self.sio.wait()
		logging.debug("connection started")
		 
    
	def run(self):
		try:
			self.start()
			self.started = True
		finally:
			self.stop()
			self.started = False
	
	def stop(self):
		self.started = False
		#if self.sio is not None:
			#self.sio.close()
	def sendMsg(self,address,var,data):
          
		args=dict()
		args[var]= data

		self.sio.emit(address,args)

"""	
		message = {'message': {
			'clientName':self.name,
			'name':publisher.name,
			'type':publisher.type,
			'value':value } }

		if options.debug: self._console.log("on: '" + name + "' published msg: " + str(message) + "\n")

		self.ws.send(json.write(message))
"""

class Console(object):
	
	def __init__(self, sio):
		self.console = socket(AF_INET, SOCK_STREAM)
		self.connected = False
		self.msg_buffer = ""
		self.sio = sio
		# self.msg_started = False
		# self.msg_completed = False
		# self.msg_timeout = 20

	def start(self):
		try:
			self.console = socket(AF_INET, SOCK_STREAM)
			self.console.connect(('localhost', 6571))
			print("console connected")
			self.connected = True
			t = threading.Thread(target=self.sio.run,)
			t.daemon = True
			t.start()
			print("connected")
			

		except:
			self.connected = False
			print("unable to connect")
			

	def read(self):
		
		if not self.connected: return

		index_end = -1

		new_data = self.console.recv(1024)
		
		
	    # if new data was received then add it buffer and check if end message was provided
		if new_data:
			self.msg_buffer += new_data			
			index_end = self.msg_buffer.find(SERIAL.MSG.END)

		if new_data == '':
			console_running = False
			self.console.close()
			return None

		# if message end was found, then look for the start and div marker
		if index_end > 0:
			index_address = self.msg_buffer.find(SERIAL.MSG.ADDR)
			index_var=self.msg_buffer.find(SERIAL.MSG.VAR)
			index_data = self.msg_buffer.find(SERIAL.MSG.DATA)
			
			
			if index_address >= 0 and index_var > index_address and index_data > index_var:
				
				try:
					address = self.msg_buffer[(index_address + 1):index_var]
					var = self.msg_buffer[(index_var + 1):index_data]
					data = self.msg_buffer[(index_data + 1):index_end]
					self.sio.sendMsg(address, var, data)
					logging.debug("address= "+address+"var= "+var+"data= "+data+" ")
					

				except Exception:
					error_msg = "issue sending message via SocketIOYunClient, route: " +address + "\n"
					self.log(error_msg)


				try:
					confirm_pub = SERIAL.MSG.CONFIRM +" "+address + SERIAL.MSG.END
					self.log(confirm_pub)

				except Exception:
					error_msg = "issue sending confirmation about: " + publish_route + "\n"
					self.log(error_msg)

			self.msg_buffer = ""
	
	def write(self, addr, var, data):
		try:
			full_msg =chr(29)+addr+ SERIAL.MSG.VAR + var + SERIAL.MSG.DATA + data +SERIAL.MSG.END
			self.console.send(full_msg)
			print(full_msg)
			print(chr(29),chr(30),chr(31))
			logging.debug("Sending to Arduino "+full_msg)
			
		except:
			pass
	
	def run(self):
		self.start()
		try:
			while True:
				if self.connected: 
					self.read()
		finally:
			self.console.close()
	
	def stop(self):
		self.started = False
		if self.ws is not None:
			self.ws.close()
	
	def log(self, message):
		self.console.send(message)

if __name__ == "__main__":
# 	if options.debug: print """
# This is the Spacebrew module. 
# See spacebrew_ex.py for usage examples.
# """

	global SIO, data, console, console_running
	open('/opt/myScripts/SocketIOYunClient/consolelog.log', 'w').close()
	logging.basicConfig(filename='/opt/myScripts/SocketIOYunClient/consolelog.log',level=logging.DEBUG)

	SIO = {}
	console = {}
	if len(sys.argv)==3:
		logging.debug("server= "+ sys.argv[1]+"port= "+str(sys.argv[2]))
		SIO = socketIOYUNClient( server=sys.argv[1], port=int(sys.argv[2]))
	else:
		logging.debug("server= "+ sys.argv[1])
		SIO = socketIOYUNClient( server=sys.argv[1])

	data = ""

	console = Console(SIO)
	
	

	SIO.console(console)

	console.run()
    
    