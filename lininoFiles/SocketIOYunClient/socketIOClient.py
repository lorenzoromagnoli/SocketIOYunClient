#!/usr/bin/env python
import sys
import logging
import websocket
import threading
from websocket import create_connection
from socketIO_client import SocketIO, BaseNamespace
import json

connected =0


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
            connected=1;
            command = raw_input('--> ')
            print("connected")
    
    if commandarray[0] == "send":
        totalArgs=len(commandarray)
        print("n arguments",totalArgs)
        
        message=commandarray[1]
        args=dict()
        for i in range(2,totalArgs-1,2):
            args[commandarray[i]]= commandarray[i+1]
        
        jsonArgs=json.dumps(args)
        print(jsonArgs)
        socketIO.emit(message,args)
        
    
while True : # this will loop until invalid_input is set to be True
    start()  