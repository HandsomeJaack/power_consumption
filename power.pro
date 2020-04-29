TEMPLATE = subdirs

SUBDIRS = \
    app \
    batstat \
    cpustat

OTHER_FILES += \
    rpm/power.spec \
    rpm/batstat.service

service.path = /etc/systemd/system/
service.files += rpm/batstat.service

INSTALLS += service

