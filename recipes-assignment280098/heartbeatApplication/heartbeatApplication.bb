DESCRIPTION = "Application to read heartbeat from dummy kernel module"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"
SRC_URI = "file://heartbeatApplication.c"
S = "${WORKDIR}"
do_compile() {
set CFLAGS -g
${CC} ${CFLAGS} -lm -pthread heartbeatApplication.c ${LDFLAGS} -o heartbeatApplication
unset CFLAGS
}
do_install() {
install -d ${D}${bindir}
install -m 0755 heartbeatApplication ${D}${bindir}
}
