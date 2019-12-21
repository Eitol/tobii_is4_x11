unofficial x11 support for the devices tobii is4 (4c). (abandoned and incomplete project)

#### Configure

The config is saved in /etc/tobii.cfg

You must put the name of the screen in this file:

Example:

screen="HDMI-1-2"

you can get the name of the screen with the command:

xrandr --listmonitors

Example out:

Monitors: 2

 0: +*eDP-1-1 1920/344x1080/193+0+504  eDP-1-1 
 
 1: +HDMI-1-2 2560/621x1440/341+1920+0  HDMI-1-2
 
In my case the device is attached to the right screen (HDMI-1-2)

#### Install all deps

1 - Install the driver: https://github.com/Eitol/tobii_eye_tracker_linux_installer

2- Install deps:  sudo bash ./install.sh

#### Demostration video:
[![Demostration video](https://img.youtube.com/vi/3wzgTG9CGRw/maxresdefault.jpg)](https://youtu.be/3wzgTG9CGRw)


# ToDo
- [x] Read device data
- [x] Create and move shape
- [x] Multi monitor support
- [x] Add configuration file support
- [x] Emulate head detection if the device doesnt support
- [-] Create the systemd and sysvinit files
- [ ] Add multi distro support (configuration file)
- [ ] Allow to configure the gaze shape

###Warning: experimental
Tested in: Ubuntu 18.10 x86_64 Cinnamon. Tobii 4C

Note: Tobii is a registered trademark.
This software does not have any relation with the company
