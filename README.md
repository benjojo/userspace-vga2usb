userspace-vga2usb
===

This is a userspace driver that uses libusb to replace some of the basic function of the offical driver for the Epiphan VGA2USB LR.

![Device](https://blog.benjojo.co.uk/asset/HxOiDPsE83)

You can find a better write up on the process of writing this here: https://blog.benjojo.co.uk/post/userspace-usb-drivers


---

Notes on OSX compatibility:

Despite me writing this driver entirely on Linux, I managed to test it at the time of completion on a Macbook with no issues in either building or run time.

To me this seems like a massive fluke, and you may need to install VirtualBox to have a working libusb that is signed or something. I've done very little testing
on OSX, other than it works on a OSX machine I had on hand.