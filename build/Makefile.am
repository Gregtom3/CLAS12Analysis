AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS = \
  -I$(CLAS12ROOT)/hipo4 \
  -I$(CLAS12ROOT)/Clas12Banks \
  -I$(CLAS12ROOT)/Clas12Root \
  -I$(CLAS12ROOT)/rcdb/cpp/include \
  -I$(ROOTSYS)/include \
  -I$(BRUFIT)/core \
  -I$(includedir) \
  -DCLAS_RCDB 

lib_LTLIBRARIES = \
   libclas12ana.la

AM_CXXFLAGS = -Wall -Werror -Wno-int-in-bool-context -Wno-reorder -Wno-class-memaccess -msse2 -std=c++14

AM_LDFLAGS = \
  -L$(CLAS12ROOT)/lib \
  -L$(HIPO)/lib \
  -L$(BRUFIT)/lib \
  -L$(libdir)

libclas12ana_la_LIBADD = \
  -lClas12Banks \
  -lClas12Root \
  -lHipo4 \
  -llz4

pkginclude_HEADERS = \
  ../src/Kinematics.h \
  ../src/Settings.h \
  ../src/SIDISKinematicsReco.h \
  ../src/SIDISParticle.h \
  ../src/Constants.h \
  ../src/PostProcess.h \
  ../src/SIDISParticlev1.h \
  ../src/FiducialCuts.h	\
  ../src/Merger.h \
  ../src/PID.h \
  ../src/Fitter.h \
  ../src/HipoBankInterface.h \
  ../src/Asymmetry.h \
  ../src/Binner.h

libclas12ana_la_SOURCES = \
  ../src/Kinematics.C \
  ../src/Settings.C \
  ../src/SIDISKinematicsReco.C \
  ../src/SIDISParticle.C \
  ../src/PostProcess.C \
  ../src/SIDISParticlev1.C \
  ../src/FiducialCuts.C \
  ../src/Merger.C \
  ../src/PID.C \
  ../src/Fitter.C \
  ../src/HipoBankInterface.C \
  ../src/Asymmetry.C \
  ../src/Binner.C

%_Dict.C: ../src/%.h ../src/%LinkDef.h
	rootcint -f $@ @CINTDEFS@ -c $(AM_CPPFLAGS) $^

%_Dict_rdict.pcm: ../src/%_Dict.C ;

clean-local:
	rm -f *Dict* $(BUILT_SOURCES) *.pcm
