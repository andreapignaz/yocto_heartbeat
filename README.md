# meta-assignment280098
Assignment for the Politecnico di Torino course, "Operating Systems for Embedded Systems"

## Target machine
These Yocto layers have been designed to work on the Raspberry Pi 3B. Compatibility has been set to that device only.
This tutorial should be done on a build environment that has been already configured for the rpi3 (layers cloned and added to bblayers.conf, local.conf and interfaces properly set).

## How to include these recipes inside the Yocto build environment
1. Clone this repository inside your Yocto directory:
```bash
git clone https://github.com/andreapignaz/meta-assignment280098.git
```
2. Go inside the rpi3 build environment:
```bash
source oe-init.build-env build_rpi3
```
3. Add the new recipes to the image to be deployed: in the file conf/local.conf inside the build_rpi3 directory, add the following lines:
```
IMAGE_INSTALL_append = "heartbeatApplication"
IMAGE_INSTALL_append = "heartbeatModule"
KERNEL_MODULE_AUTOLOAD += "heartbeatModule"
```
4. Run bitbake:
```
bitbake core-image-full-cmdline 
```
