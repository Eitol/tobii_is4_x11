unofficial x11 support for the devices tobii is4 (4c).

##### Install all deps

1 - Install the driver: https://github.com/Eitol/tobii_eye_tracker_linux_installer

2- Install deps:  sudo bash ./install.sh

####Demostration video:
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
