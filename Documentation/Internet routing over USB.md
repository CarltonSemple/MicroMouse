##Internet Routing over USB

-----------------
> Caveat: These instructions assume that you have a Linux (probably Ubuntu) VM with internet access. 

####Accessing the Beagle
When you first connect the beagle to Ubuntu the "boot" USB image will mount, but you won't see the beagle on the network. You need to open up your Network setting, where you will see a new "Wired" device, with the switch in the OFF position. Turn it on, then click "Options..." and go to the IPv4 Settings tab. The method should be "Automatic (DHCP)".  In the DHCP client ID field enter: `192.168.7.2`, then save.

Once this is done, open a terminal and login to the beagle with the following:

```
$ ssh ubuntu@192.168.7.2
$ password: temppwd
```
	
####Modifying Network Interfaces
Once logged in, you must edit the Network Interface file. 

	$ sudo nano etc/network/interfaces
	
Make this section of the file resemble: 

	auto usb0
	iface usb0 inet static
		address 192.168.7.2
		netmask 255.255.255.0
		gateway 192.168.7.1
		dns-nameservers 8.8.8.8 8.8.4.4
	
Reboot the BeagleBone and enter the following after ssh-ing back in:

	$  route add default gw 192.168.7.1

> Note:  You might have to repeat this last command every now and then, after which you must disconnect and then reconnect with the Beagle.

####Sharing Connection of Host

Open up the terminal and enter the following:

* LINUX

```
$ echo 1 > /proc/sys/net/ipv4/ip_forward
$ iptables -A POSTROUTING -t nat -j MASQUERADE
```
* MAC

```
$ sysctl -w net.inet.ip.forwarding=1
```

####Check Connction on Beagle

	$ ping www.google.com
	