TEMPLATE = subdirs

SUBDIRS = QGLViewer OGLRender \
    Ray_Tracing

 # what subproject depends on others
Transfos.depends = QGLViewer OGLRender
Revolution.depends = QGLViewer OGLRender
Projet_modeling.depends = QGLViewer OGLRender

