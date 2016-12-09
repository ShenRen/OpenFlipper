OpenFlipper README.TXT
=======================
The documentation is on our website:
http://www.openflipper.org

# Getting Source Code from git
Clone the repository with submodules:   
 git clone --recursive https://www.graphics.rwth-aachen.de:9000/OpenFlipper-Free/OpenFlipper-Free.git
 
# Required libs: 
Qt ( >= 4.8.2 ) http://www.qtsoftware.com/downloads  
OpenGL          http://www.opengl.org  
GLUT            http://www.opengl.org/resources/libraries/glut  
GLEW            http://glew.sourceforge.net  
qwt ( >=6.0 )   optional http://qwt.sourceforge.net/  

# Compiling OpenFlipper:
A detailed compilation manual is available online at:
http://openflipper.org/Documentation/latest/buildingOpenFlipper.html

Latest NEWS
===========

- 12/09/2016: Moved OpenMesh directory into OpenFlipper subdirectory OpenFlipper/libs_required/OpenMesh
	      ** You might have to delete your libs_required/OpenMesh directory manually if you get cmake errors regarding duplicate targets **
- 12/09/2016: Moved ACG directory into OpenFlipper subdirectory OpenFlipper/libs_required/ACG
- 12/08/2016: Moved CI directory into OpenFlipper subdirectory to remove toplevel dir count
- 12/08/2016: Moved the cmake directory contents into OpenFlipper/cmake (First steps to reduce number of toplevel dirs and get more modular than before)
