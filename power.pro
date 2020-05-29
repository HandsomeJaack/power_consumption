TEMPLATE = subdirs

SUBDIRS = \
    app \
    batstat \
    voltstat \
    tests \

OTHER_FILES += \
    rpm/power.spec \
    rpm/batstat.service \
    rpm/voltstat.service \

service_bat.path = /etc/systemd/system/
service_bat.files += rpm/batstat.service

service_volt.path += /etc/systemd/system
service_volt.files += rpm/voltstat.service

INSTALLS += service_bat service_volt

