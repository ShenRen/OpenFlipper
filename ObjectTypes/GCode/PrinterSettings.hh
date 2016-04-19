#pragma once

#include <string>
#include "EdgeTypes.hh"

#include <ACG/Geometry/Algorithms.hh>
#include <QCoreApplication>
#include <QString>
#include <QSettings>

namespace Ultimaker{

struct PrinterSettings{
    enum GCodeFlavor {GCode_Ultimaker = 0, GCode_Makerbot = 1};

    PrinterSettings() :
        name("Unknown"),
        filament_diameter(2.85),
        nozzle_size(0.4),
        nozzle_overlap(0.0),
        zero_layer(0.3),
        size(1.0, 1.0, 1.0),
        center(0.0, 0.0, 0.0),
        flavor(GCode_Ultimaker),
        speed_first_layer(10.0),
        flow_first_layer(120.0)
    {
        speeds[Ultimaker::GC_MOVE] = 70.0;
        speeds[Ultimaker::GC_WALL_OUTER] = 20.0;
        speeds[Ultimaker::GC_WALL_INNER] = 30.0;
        speeds[Ultimaker::GC_INFILL] = 40.0;
        speeds[Ultimaker::GC_TOPBOTTOM] = 40.0;
        speeds[Ultimaker::GC_SUPPORT_ACC] = 40.0;
        speeds[Ultimaker::GC_SUPPORT_ACC_LE] = 40.0;
        speeds[Ultimaker::GC_SUPPORT] = 30.0;
        speeds[Ultimaker::GC_BRIM] = 20.0;

        flows[Ultimaker::GC_MOVE] = 100.0;
        flows[Ultimaker::GC_WALL_OUTER] = 100.0;
        flows[Ultimaker::GC_WALL_INNER] = 100.0;
        flows[Ultimaker::GC_INFILL] = 100.0;
        flows[Ultimaker::GC_TOPBOTTOM] = 100.0;
        flows[Ultimaker::GC_SUPPORT_ACC] = 60.0;
        flows[Ultimaker::GC_SUPPORT_ACC_LE] = 45.0;
        flows[Ultimaker::GC_SUPPORT] = 100.0;
        flows[Ultimaker::GC_BRIM] = 300.0;
    }

    std::string name; // Name of the printer

    double filament_diameter; // Diameter of the used filament
    double nozzle_size; // Nozzle size
    double nozzle_overlap; // Overlap between lines
    double zero_layer; // First layer height to print

    ACG::Vec3d size;
    ACG::Vec3d center;

    GCodeFlavor flavor;

    double speed_first_layer;
    double flow_first_layer;

    double speeds[Ultimaker::GC_COUNT]; // Speeds in mm/s
    double flows[Ultimaker::GC_COUNT]; // Flows in percent

    void load(const std::string& _filename){
        QSettings settings(QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg(QString::fromStdString(_filename)), QSettings::IniFormat);
        name = settings.value("name").toString().toStdString();

        filament_diameter = settings.value("filament_diameter").toDouble();
        nozzle_size = settings.value("nozzle_size").toDouble();
        nozzle_overlap = settings.value("nozzle_overlap").toDouble();
        zero_layer = settings.value("zero_layer").toDouble();

        size[0] = settings.value("size_x").toDouble();
        size[1] = settings.value("size_y").toDouble();
        size[2] = settings.value("size_z").toDouble();

        center[0] = settings.value("center_x").toDouble();
        center[1] = settings.value("center_y").toDouble();
        center[2] = settings.value("center_z").toDouble();

        speeds[Ultimaker::GC_MOVE] = settings.value("speed_move", 70.0).toDouble();
        speeds[Ultimaker::GC_WALL_OUTER] = settings.value("speed_wallouter", 20.0).toDouble();
        speeds[Ultimaker::GC_WALL_INNER] = settings.value("speed_wallinner", 30.0).toDouble();
        speeds[Ultimaker::GC_INFILL] = settings.value("speed_infill", 40.0).toDouble();
        speeds[Ultimaker::GC_TOPBOTTOM] = settings.value("speed_topbottom", 40.0).toDouble();
        speeds[Ultimaker::GC_SUPPORT_ACC] = settings.value("speed_support_acc", 40.0).toDouble();
        speeds[Ultimaker::GC_SUPPORT_ACC_LE] = settings.value("speed_support_acc_le", 40.0).toDouble();
        speeds[Ultimaker::GC_SUPPORT] = settings.value("speed_support", 40.0).toDouble();
        speeds[Ultimaker::GC_BRIM] = settings.value("speed_brim", 20.0).toDouble();
        speed_first_layer = settings.value("speed_firstlayer", 10.0).toDouble();

        flows[Ultimaker::GC_MOVE] = settings.value("flow_move", 100.0).toDouble();
        flows[Ultimaker::GC_WALL_OUTER] = settings.value("flow_wallouter", 100.0).toDouble();
        flows[Ultimaker::GC_WALL_INNER] = settings.value("flow_wallinner", 100.0).toDouble();
        flows[Ultimaker::GC_INFILL] = settings.value("flow_infill", 100.0).toDouble();
        flows[Ultimaker::GC_TOPBOTTOM] = settings.value("flow_topbottom", 100.0).toDouble();
        flows[Ultimaker::GC_SUPPORT_ACC] = settings.value("flow_support_acc", 60.0).toDouble();
        flows[Ultimaker::GC_SUPPORT_ACC_LE] = settings.value("flow_support_acc_le", 45.0).toDouble();
        flows[Ultimaker::GC_SUPPORT] = settings.value("flow_support", 100.0).toDouble();
        flows[Ultimaker::GC_BRIM] = settings.value("flow_brim", 300.0).toDouble();
        flow_first_layer = settings.value("flow_firstlayer", 110.0).toDouble();

        flavor = GCodeFlavor(settings.value("flavor").toInt());
    }

    void save(const std::string& _filename){
        QSettings settings(QString("%1/%2").arg(QCoreApplication::applicationDirPath()).arg(QString::fromStdString(_filename)), QSettings::IniFormat);
        settings.setValue("name", QString::fromStdString(name));

        settings.setValue("filament_diameter", filament_diameter);
        settings.setValue("nozzle_size", nozzle_size);
        settings.setValue("nozzle_overlap", nozzle_overlap);
        settings.setValue("zero_layer", zero_layer);

        settings.setValue("size_x", size[0]);
        settings.setValue("size_y", size[1]);
        settings.setValue("size_z", size[2]);

        settings.setValue("center_x", center[0]);
        settings.setValue("center_y", center[1]);
        settings.setValue("center_z", center[2]);

        settings.setValue("speed_move", speeds[Ultimaker::GC_MOVE]);
        settings.setValue("speed_wallouter", speeds[Ultimaker::GC_WALL_OUTER]);
        settings.setValue("speed_wallinner", speeds[Ultimaker::GC_WALL_INNER]);
        settings.setValue("speed_infill", speeds[Ultimaker::GC_INFILL]);
        settings.setValue("speed_topbottom", speeds[Ultimaker::GC_TOPBOTTOM]);
        settings.setValue("speed_support_acc", speeds[Ultimaker::GC_SUPPORT_ACC]);
        settings.setValue("speed_support_acc_le", speeds[Ultimaker::GC_SUPPORT_ACC_LE]);
        settings.setValue("speed_support", speeds[Ultimaker::GC_SUPPORT]);
        settings.setValue("speed_brim", speeds[Ultimaker::GC_BRIM]);
        settings.setValue("speed_firstlayer", speed_first_layer);

        settings.setValue("flow_move", flows[Ultimaker::GC_MOVE]);
        settings.setValue("flow_wallouter", flows[Ultimaker::GC_WALL_OUTER]);
        settings.setValue("flow_wallinner", flows[Ultimaker::GC_WALL_INNER]);
        settings.setValue("flow_infill", flows[Ultimaker::GC_INFILL]);
        settings.setValue("flow_topbottom", flows[Ultimaker::GC_TOPBOTTOM]);
        settings.setValue("flow_support_acc", flows[Ultimaker::GC_SUPPORT_ACC]);
        settings.setValue("flow_support_acc_le", flows[Ultimaker::GC_SUPPORT_ACC_LE]);
        settings.setValue("flow_support", flows[Ultimaker::GC_SUPPORT]);
        settings.setValue("flow_brim", flows[Ultimaker::GC_BRIM]);
        settings.setValue("flow_firstlayer", flow_first_layer);

        settings.setValue("flavor", int(flavor));
    }
};

}
