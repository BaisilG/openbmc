FILESEXTRAPATHS_prepend := "${THISDIR}/files/pal:"

SRC_URI += "file://pal.c \
            file://pal.h \
            file://pal_sensors.c \
            file://pal_sensors.h \
            file://pal_health.c \
            file://pal_health.h \
            "
HEADERS += "pal_health.h"
SOURCES += "pal_sensors.c pal_health.c"

DEPENDS += "libgpio-ctrl libobmc-i2c switchtec-user libobmc-sensors"
RDEPENDS_${PN} += " libgpio-ctrl libobmc-i2c switchtec-user libobmc-sensors"
LDFLAGS += " -lgpio-ctrl -lobmc-i2c -lswitchtec -lobmc-sensors"
