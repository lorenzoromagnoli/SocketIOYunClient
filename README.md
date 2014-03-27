#A library to connect arduinoYun to nodeJS via a socket.io protocol#
The library is composed of a Python script that make use of socketIO-client to connect to a socket.io sever and an Arduino Library to communicate to the Python script.

The following guide and library is based on the work of [Tobias Oberstein](https://github.com/oberstet)  [Getting started with Arduino Yun and Autobahn](http://tavendo.com/blog/post/arduino-yun-with-autobahn/)

You will need an arduino YUN properly configured to connected to the Internet

To configure the YUN for 802.x network like eduroam follow this guide 	[http://forum.arduino.cc/index.php?topic=197267.0](http://forum.arduino.cc/index.php?topic=197267.0)
Thanks bitfrosting

##how to install the library?##

####Connect to the Arduino Yun via ssh####

	ssh root@myYun.local

####Update the packages####

	opkg update

####Install a sftp server ####

We want to install openssh-sftp-server so that we can connect to the yun via our favorite ftp software (fliezilla, cyberduck, etc) 

	opkg install openssh-sftp-server

if it doesn't find the package you have tu update the repository list, open the opkg configuration file with

	vi /etc/opkg.conf

and change the first line to

	src/gz barrier_breaker http://download.linino.org/dogstick/all-in-one/latest/packages/

your opkg.conf now should look like this

	src/gz barrier_breaker http://download.linino.org/dogstick/all-in-one/latest/package
	dest root /
	dest ram /tmp
	lists_dir ext /var/opkg-lists
	option overlay_root /overlay
	option check_signature

now you can install openssh-sftp-server

####More packages ####

let's install other packages we will need

	opkg install bzip2
	opkg install unzip
	opkg install tar
	opkg install wget
	opkg install fdisk
	opkg install e2fsprogs
	opkg install openssh-sftp-server
	opkg install distribute
	opkg install pyopenssl
	opkg install python-openssl
	opkg install python-crypto
	opkg install python-bzip2
	opkg install python-sqlite3
	opkg install python-ncurses		

This might take some time, but the packages are installed into the / root filesystem and will be permanently available from now on.	
 
####Format the SD card####
 
The / filesystem on the Yun only has 8MB space in total (the other 8MB of the total 16MB Flash are used for system recovery)
The free space on / isn't enough for the software we will use. And we also want to have space for local data storage. Hence we will create a new filesystem on a micro SD card which we can use for both programs and data.

Make sure you have your SD card inserted. Then, create a new ext4 filesystem on the SD card (WARNING: this will erase all data on that card without any further questions!):

	umount -f /mnt/sda1
	mkfs.ext4 -O ^has_journal,extent /dev/sda1
	mkdir /opt

To manually mount the filesystem you could do

		mount -t ext4 /dev/sda1 /opt

A better way however is to have the filesystem automatically mounted at startup of the Yun. For that, add the following new entry to the filesystems configuration in the file /etc/config/fstab:

	config mount
    option target       /opt
    option device       /dev/sda1
    option fstype       ext4
    option options      rw,async,noatime
    option enabled      1
    option enabled_fsck 1

To check size and free space

	df -h

####Creating a Python Virtualenv####
Virtualenv is a widely used Python tool that allows to create complete Python environments in different locations and with different setups. We will use Virtualenv to create a Python enviroment on our SD card under /opt/python.

	/usr/bin/easy_install virtualenv

Now create a new Python environment on /opt/python (residing on our SD card):

	/usr/bin/virtualenv --system-site-packages /opt/python

After that, you should have a new Python here:

	root@miniverse:~# /opt/python/bin/python
	Python 2.7.3 (default, Aug  8 2013, 22:36:42)
	[GCC 4.6.4 20121210 (prerelease)] on linux2
	Type "help", "copyright", "credits" or "license" for more information.
	>>>

From now on, you have two sets of Python binaries, the system Python:

	root@Arduino:~# which python
	/usr/bin/python
	root@Arduino:~# which easy_install
	/usr/bin/easy_install


Make sure you use the full path to the Python (or easy_install) you want to use:

	/opt/python/bin/python
	/opt/python/bin/easy_install

or activate the virtualenv by doing

	root@Arduino:~# source /opt/python/bin/activate
	(python)root@Arduino:~# which python
	/opt/python/bin/python
	(python)root@Arduino:~# which easy_install
	/opt/python/bin/easy_install

> *This activation will simply modify your shell environment appropriately, so it isn't anything persisting. Personally, I usually don't bother with virtualenv environment activation, but simply use fully qualified paths.*

####Install socketIO-client  ####
The python library we are going to use is based on [socketIO-client](https://pypi.python.org/pypi/socketIO-client).
once we have activated our vistual evironment with:

	source /opt/python/bin/activate

we can use pip to install the package, it will automatically install all the required dependencies

	pip install -U socketIO-client

Spacebrew library for arduino is using a python websocket library as well. Somehow, even if just created a python visrtual environment, the websocket library installed in the main python library at `/usr/lib/python2.7/websocket' conflicts with the one that we need installed in our `/opt/python/lib/python2.7/site-packages`.

My temporary solution is to remove the websocket folder from the root python:

	rm -rf /usr/lib/python2.7/websocket/		

####Install the python script on the YUN####
Start by downloading this repository.
use your favourite ftp software to open a connection with your board

	protocol: sftp
	address: [yourArduinoYUNName].local
	user:root
	psw:[yourArduinoYUNPassword]

Create a folder called `myScripts` in `/opt/`. 

> *you might use this folder in the future for other python scripts*

copy the entire folder `/linino files/SocketIOYunClient` from the repository to the folder `/opt/myScripts/`

you will need to make  `/opt/myScripts/SocketIOYunClient/socketIOClient.sh` executable.

	chmod +x /opt/myScripts/SocketIOYunClient/socketIOClient.sh

####Finally install the Arduino Library####
To program the arduino yun the beta version of the IDE is needed.
ThisOne usually creates a new sketch folder. 
Copy the repository to `[your Arduino-Beta sketch folder]/libraries/
Restart your IDE and you are good to go!

