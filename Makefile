#############################################################################
# Makefile for building: ts_wakeup
# Generated by qmake (2.01a) (Qt 4.7.2) on: Mon Sep 19 10:07:29 2016
# Project:  ts_wakeup.pro
# Template: app
# Command: /usr/local/QtEmbedded-4.7.2-arm/bin/qmake -spec /usr/local/QtEmbedded-4.7.2-arm/mkspecs/qws/linux-mxc-g++ CONFIG+=debug BOARDTYPE=IMX6 -o Makefile ts_wakeup.pro
#############################################################################

####### Compiler, tools and options

CC            = arm-none-linux-gnueabi-gcc
CXX           = arm-none-linux-gnueabi-g++
DEFINES       = -DIMX6 -DIMX6 -DQT_GUI_LIB -DQT_NETWORK_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -Wall -W -D_REENTRANT $(DEFINES)
CXXFLAGS      = -pipe -Wno-psabi -g -Wall -W -D_REENTRANT $(DEFINES)
INCPATH       = -I/usr/local/QtEmbedded-4.7.2-arm/mkspecs/qws/linux-mxc-g++ -I. -I/usr/local/QtEmbedded-4.7.2-arm/include/QtCore -I/usr/local/QtEmbedded-4.7.2-arm/include/QtNetwork -I/usr/local/QtEmbedded-4.7.2-arm/include/QtGui -I/usr/local/QtEmbedded-4.7.2-arm/include -I../../../ltib/rootfs/usr/include/gstreamer-0.10 -I../../../ltib/rootfs/usr/include/glib-2.0 -I../../../ltib/rootfs/usr/lib/glib-2.0/include -I../../../ltib/rootfs/usr/include/libxml2 -I../../../ltib/rootfs/usr/include -I. -I/usr/local/QtEmbedded-4.7.2-arm/include -I../../../ltib/rootfs/usr/include
LINK          = arm-none-linux-gnueabi-g++
LFLAGS        = -Wl,-rpath,/usr/local/QtEmbedded-4.7.2-arm/lib
LIBS          = $(SUBLIBS)  -L/usr/local/QtEmbedded-4.7.2-arm/lib -L/home/standby/doorentry/ltib/rootfs/usr/lib -L/home/standby/doorentry/ltib/rootfs/usr/lib -lgstreamer-0.10 -lgstapp-0.10 -lgstbase-0.10 -lglib-2.0 -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lxml2 -lopencv_core -lopencv_imgproc -lopencv_video -lopencv_highgui -lopencv_objdetect -lopencv_contrib -lopencv_flann -lopencv_features2d -lopencv_calib3d -lopencv_ml -lopencv_legacy -lopencv_photo -lopencv_gpu -lopencv_ocl -lopencv_nonfree -lts -lz -lQtGui -L/usr/local/QtEmbedded-4.7.2-arm/lib -lQtNetwork -lQtCore -lpthread 
AR            = arm-none-linux-gnueabi-ar cqs
RANLIB        = 
QMAKE         = /usr/local/QtEmbedded-4.7.2-arm/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = $(COPY)
COPY_DIR      = $(COPY) -r
STRIP         = arm-none-linux-gnueabi-strip
INSTALL_FILE  = install -m 644 -p
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = install -m 755 -p
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = main.cpp \
		stream.cpp \
		background_detect.cpp \
		detectobject.cpp moc_stream.cpp
OBJECTS       = main.o \
		stream.o \
		background_detect.o \
		detectobject.o \
		moc_stream.o
DIST          = /usr/local/QtEmbedded-4.7.2-arm/mkspecs/common/g++.conf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/common/unix.conf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/common/linux.conf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/common/qws.conf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/qconfig.pri \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/qt_functions.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/qt_config.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/exclusive_builds.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/default_pre.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/debug.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/default_post.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/warn_on.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/qt.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/unix/thread.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/moc.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/resources.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/uic.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/yacc.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/lex.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/include_source_dir.prf \
		ts_wakeup.pro
QMAKE_TARGET  = ts_wakeup
DESTDIR       = 
TARGET        = ts_wakeup

first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: ts_wakeup.pro  /usr/local/QtEmbedded-4.7.2-arm/mkspecs/qws/linux-mxc-g++/qmake.conf /usr/local/QtEmbedded-4.7.2-arm/mkspecs/common/g++.conf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/common/unix.conf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/common/linux.conf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/common/qws.conf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/qconfig.pri \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/qt_functions.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/qt_config.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/exclusive_builds.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/default_pre.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/debug.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/default_post.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/warn_on.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/qt.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/unix/thread.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/moc.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/resources.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/uic.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/yacc.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/lex.prf \
		/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/include_source_dir.prf \
		/usr/local/QtEmbedded-4.7.2-arm/lib/libQtGui.prl \
		/usr/local/QtEmbedded-4.7.2-arm/lib/libQtNetwork.prl \
		/usr/local/QtEmbedded-4.7.2-arm/lib/libQtCore.prl
	$(QMAKE) -spec /usr/local/QtEmbedded-4.7.2-arm/mkspecs/qws/linux-mxc-g++ CONFIG+=debug BOARDTYPE=IMX6 -o Makefile ts_wakeup.pro
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/common/g++.conf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/common/unix.conf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/common/linux.conf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/common/qws.conf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/qconfig.pri:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/qt_functions.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/qt_config.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/exclusive_builds.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/default_pre.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/debug.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/default_post.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/warn_on.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/qt.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/unix/thread.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/moc.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/resources.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/uic.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/yacc.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/lex.prf:
/usr/local/QtEmbedded-4.7.2-arm/mkspecs/features/include_source_dir.prf:
/usr/local/QtEmbedded-4.7.2-arm/lib/libQtGui.prl:
/usr/local/QtEmbedded-4.7.2-arm/lib/libQtNetwork.prl:
/usr/local/QtEmbedded-4.7.2-arm/lib/libQtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/local/QtEmbedded-4.7.2-arm/mkspecs/qws/linux-mxc-g++ CONFIG+=debug BOARDTYPE=IMX6 -o Makefile ts_wakeup.pro

dist: 
	@$(CHK_DIR_EXISTS) .tmp/ts_wakeup1.0.0 || $(MKDIR) .tmp/ts_wakeup1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/ts_wakeup1.0.0/ && $(COPY_FILE) --parents stream.h background_detect.h detectobject.h .tmp/ts_wakeup1.0.0/ && $(COPY_FILE) --parents main.cpp stream.cpp background_detect.cpp detectobject.cpp .tmp/ts_wakeup1.0.0/ && (cd `dirname .tmp/ts_wakeup1.0.0` && $(TAR) ts_wakeup1.0.0.tar ts_wakeup1.0.0 && $(COMPRESS) ts_wakeup1.0.0.tar) && $(MOVE) `dirname .tmp/ts_wakeup1.0.0`/ts_wakeup1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/ts_wakeup1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) $(TARGET) 
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_moc_header_make_all: moc_stream.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_stream.cpp
moc_stream.cpp: stream.h
	/usr/local/QtEmbedded-4.7.2-arm/bin/moc $(DEFINES) $(INCPATH) stream.h -o moc_stream.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_uic_make_all:
compiler_uic_clean:
compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean 

####### Compile

main.o: main.cpp stream.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o main.cpp

stream.o: stream.cpp stream.h \
		background_detect.h \
		detectobject.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o stream.o stream.cpp

background_detect.o: background_detect.cpp background_detect.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o background_detect.o background_detect.cpp

detectobject.o: detectobject.cpp detectobject.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o detectobject.o detectobject.cpp

moc_stream.o: moc_stream.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_stream.o moc_stream.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

