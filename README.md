# meta-assignment280098
Assignment for the Politecnico di Torino course, "Operating Systems for Embedded Systems"

## Target machine
This Yocto layer has been designed (and tested) to work on the Raspberry Pi 3B.

These steps should be done on an already set up Raspberry Pi 3 build environment.
  

## Add the recipes to Yocto and build the Linux image
1. Clone this repository inside your Yocto directory:
```bash
git clone https://github.com/andreapignaz/meta-assignment280098.git
```
2. Go inside the rpi3 build environment:
```bash
source oe-init-build-env build_rpi3
```
3. Add the cloned layer to the rpi3 building container:
```bash
bitbake-layers add-layer ../meta-assignment280098
```
4. Add the new recipes to the image to be deployed: in the file conf/local.conf inside the build_rpi3 directory, add the following lines:
```
IMAGE_INSTALL_append = " heartbeatapplication"
IMAGE_INSTALL_append = " heartbeatmodule"
KERNEL_MODULE_AUTOLOAD += "heartbeatmodule"
```
5. Run bitbake:
```bash
bitbake core-image-full-cmdline 
```
6. Flash the microSD with the image (replace sdX with the correct device identifier):
```bash
sudo dd if=tmp/deploy/images/raspberrypi3/core-image-full-cmdline-raspberrypi3.rpi-sdimg of=/dev/sdX bs=1M
```

## Use the driver and the application
After the Raspberry Pi did boot up:
1.  Launch the heartbeat application:
```bash
heartbeatapplication
```
