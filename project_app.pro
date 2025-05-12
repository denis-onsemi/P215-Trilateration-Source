



# !!!   BUILDING  NEW  VERSION !!!
#************************************************************
# Please set prebuild command in projects setup
# chelper.py version
# => example "Custom Process Step: python.exe chelper.py version"
#************************************************************
# and post build command
# chelper.py copy
# => example "Custom Process Step: python.exe chelper.py copy"
#************************************************************










#############################################################
# PROJECT SPECIFIC DATA
#############################################################

TARGET = DSI3PAS #example P215,C516

#DEFINES += ACTIVEX
#DEFINES  += USE_FTDI
#DEFINES  += USE_CONTROLLER
DEFINES   += USE_SERPORT
#DEFINES  += USE_VEC_INTERPRETER
#DEFINES += USE_SCANCHAIN
DEFINES += HIDE


PROJECTPATH = $$PWD
BENCHPATH = ./bench/
INCLUDEPATH += $$PROJECTPATH

CONFIG += resources_big

#########USERS PART###############

# The project file name *.pro  doesn't strictly limited
# project_app.qrc - default project resource
# project_app.cpp - dafault project cpp file
# project_app.h -default project  header file


SOURCES += \
    dsi3master.cpp\
    asicmaster.cpp\
    rdummaster.cpp\
    dsi3slave.cpp\
    measurement.cpp \
    uartuartcontroller.cpp \
    uartspicontroller.cpp \
    uartssicontroller.cpp \
    CFARsim.cpp


HEADERS += \
    project_app.h\
    dsi3master.h\
    asicmaster.h\
    rdummaster.h\
    dsi3slave.h\
    dsi3slave_regmap.h\
    measurement.h \
    measurement_regmap.h \
    uartuartcontroller.h \
    uartspicontroller.h \
    uartssicontroller.h \
    CFARsim.h


FORMS += \
    dsi3master.ui\
    asicmaster.ui\
    rdummaster.ui\
    dsi3slave.ui\
    measurement.ui \
    uartuartcontroller.ui \
    uartspicontroller.ui \
    uartssicontroller.ui \
    CFARsim.ui
RESOURCES += \
    $$PWD/project_app.qrc

include ($$BENCHPATH/bench.pri)

#########USERS PART###############

OTHER_FILES += \
    README.TXT \
    changes.txt
