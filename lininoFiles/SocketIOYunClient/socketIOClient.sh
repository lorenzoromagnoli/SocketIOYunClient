
#!/bin/bash
source /opt/python/bin/activate
which python
echo "VAFFA" 
python /opt/myScripts/SocketIOYunClient/socketIOClient.py $@
