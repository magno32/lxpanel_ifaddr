lxpanel_ifaddr
==============

LXDE Panel Plugin that displays an interfaces current IP address

We needed a simple LXDE Plugin to display the IP information of a
kiosk style machine our company is using, since it may not have a static IP
address once installed.  Since I could not find one (though I did not look TOO hard) I used the lxpanel plugin tutorial (http://wiki.lxde.org/en/How_to_write_plugins_for_LXPanel) to write my own.  Please refer to that wiki page to get the required packages in order to build.

I did this on Ubuntu Server 12.04, and have not tested anywhere else.

Currently (at the time I am adding this) the interface name is hardcoded
to 'eth0'.

Quick build instructions (once you have the dependencies):

- Install
 1. make all
 2. sudo make install
- Uninstall
 1. sudo make uninstall

The plugin should be accessible from the "Add/Remove Panel Items" option in LXDE

