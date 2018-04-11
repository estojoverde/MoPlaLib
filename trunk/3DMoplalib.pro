QT += widgets core gui opengl

HEADERS += \
    mpl_baseenvironment.h \
    mpl_gridenvironment.h \
    mpl_khatibpotentialfield.h \
    mpl_harmonicpotentialfield.h \
    mpl_orientedpotentialfield.h \
    mpl_locallyorientedpotentialfield.h \
    mpl_wavefront.h \
    mpl_potentialfieldenvironment.h \
    mpl_position.h \
    mpl_utils.h \
    mpl_testsuite.h \
    mpl_core.h \
    mpl_opf_parallelsor_redblack.h \
    mpl_opf_parallelsor_mainlaucher.h \
    vis_scene3d.h \
    vis_mainwindow.h \
    vis_vectorfieldviewer.h

SOURCES += \
    mpl_baseenvironment.cpp \
    mpl_gridenvironment.cpp \
    mpl_khatibpotentialfield.cpp \
    mpl_harmonicpotentialfield.cpp \
    mpl_orientedpotentialfield.cpp \
    mpl_locallyorientedpotentialfield.cpp \
    mpl_wavefront.cpp \
    mpl_potentialfieldenvironment.cpp \
    mpl_position.cpp \
    main.cpp \
    mpl_testsuite.cpp \
    mpl_core.cpp \
    mpl_opf_parallelsor_redblack.cpp \
    mpl_opf_parallelsor_mainlaucher.cpp \
    vis_vectorfieldviewer.cpp \
    vis_scene3d.cpp \
    vis_mainwindow.cpp

OTHER_FILES += \
    mainpage.dox



unix {
LIBS += -lGL #-lGLEW
}

win32 {
LIBS += -lopengl32 #-lglew32s
}

#QMAKE_CXXFLAGS += -E

RESOURCES += \
    icons.qrc

DISTFILES += \
    ../TODO
