#pragma once
#ifndef STL_PARSER_H
#define STL_PARSER_H

#include "vtk-8.2/vtkPolyData.h"
#include "vtk-8.2/vtkAppendPolyData.h"
#include "vtk-8.2/vtkCellArray.h"
#include "vtk-8.2/vtkCellData.h"
#include "vtk-8.2/vtkSTLReader.h"
#include "vtk-8.2/vtkPLYWriter.h"
#include "vtk-8.2/vtkSmartPointer.h"
#include "vtk-8.2/vtkPolyDataMapper.h"
#include "vtk-8.2/vtkActor.h"
#include "vtk-8.2/vtkRenderWindow.h"
#include "vtk-8.2/vtkRenderer.h"
#include "vtk-8.2/vtkRenderWindowInteractor.h"
#include "vtk-8.2/vtkNamedColors.h"
#include "vtk-8.2/vtkNew.h"
#include "vtk-8.2/vtkProperty.h"
#include "vtk-8.2/vtkCamera.h"
#include "vtk-8.2/vtkPNGWriter.h"
#include "vtk-8.2/vtkBMPWriter.h"
#include "vtk-8.2/vtkJPEGWriter.h"
#include "vtk-8.2/vtkTIFFWriter.h"
#include "vtk-8.2/vtkImageWriter.h"
#include "vtk-8.2/vtkWindowToImageFilter.h"
#include "vtk-8.2/vtkImageCanvasSource2D.h"
#include "vtk-8.2/vtkImageCast.h"

//////////////////////////////////////////////////////
// 如果不是用CMake编译，而是用VS，需要加这三行。    //
#include "vtk-8.2/vtkAutoInit.h"
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingOpenGL2);
//////////////////////////////////////////////////////

#include "program_exception.h"
#include "json_parser.h"
#include "xml_parser.h"

#include <map>

class STLParser // 单例，用于解析 STL 文件，生成 PNG 图片、u3d 文件等
{
public:
    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkAppendPolyData> append_polydata;

    bool has_read_stl; // 已执行过 load_stl_files()

public:
	static STLParser* get_instance();

    void load_stl_files();

    void show_models();

    void capture_views();

    void write_image(string const& name, string const& type, const int& scale, vtkRenderWindow* renWin, bool rgba=false);

    void write_label_image(string const& name, const double rgb[3]);

    void write_ply();

private:
    STLParser();
    ~STLParser();
};

#endif // !STL_PARSER_H

