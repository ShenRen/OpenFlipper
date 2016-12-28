include (plugin)

openflipper_plugin (INSTALLDATA Icons  
                    DIRS OpenMesh OpenVolumeMesh Widgets 
                    DEPS OpenMesh OpenVolumeMesh
                    OPT_TYPES SKELETON BSPLINECURVE POLYHEDRALMESH HEXAHEDRALMESH TETRAHEDRALMESH
                    TYPES POLYMESH TRIANGLEMESH)
