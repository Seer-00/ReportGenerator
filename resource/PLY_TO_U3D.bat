@echo off

:: 根据需要编辑以下路径

:: 1. 在程序中调用时，"."表示项目路径。
set input_ply="./output/temp/enabled_models.ply"
set output_u3d="./output/temp/enabled_models.u3d"
set script_file="./resource/Simplification.mlx"
set meshlabserver="D:/Program Files/VCG/MeshLab/meshlabserver.exe"

:: 2. 双击运行该bat文件时，"."表示bat所在目录。
::set input_ply="../output/temp/enabled_models.ply"
::set output_u3d="../output/temp/enabled_models.u3d"
::set script_file="./Simplification.mlx"
::set meshlabserver="D:/Program Files/VCG/MeshLab/meshlabserver.exe"

:: STL经VTK渲染保存为PLY文件，颜色渲染的是 face color（即 fc）
set options="fc ff fq fn"

echo "Converting (%input_ply%) to (%output_u3d%) with script (%script_file%)."

%meshlabserver% -i %input_ply% -o %output_u3d% -m %options% -s %script_file%

::pause


:: meshlabserver usage:
::	 meshlabserver arg1 arg2 ...  
:: where args can be: 
::   -i [filename...]  mesh(s) that has to be loaded
::   -o [filename...]  mesh(s) where to write the result(s)
::   -s filename	   script to be applied
::   -d filename       dump on a text file a list of all the filtering fucntion
::   -l filename       the log of the filters is ouput on a file
::   -om options       data to save in the output files: 
::						vc -> vertex colors, vf -> vertex flags, vq -> vertex quality, vn-> vertex normals, vt -> vertex texture coords,
::						fc -> face colors, ff -> face flags, fq -> face quality, fn-> face normals, 
::						wc -> wedge colors, wn-> wedge normals, wt -> wedge texture coords 

:: example:
::	 'meshlabserver -i input.obj -o output.ply -s meshclean.mlx -om vc fq wn'

:: notes:
::	 There can be multiple meshes loaded and the order they are listed matters because 
::	 filters that use meshes as parameters choose the mesh based on the order.
::	 The number of output meshes must be either one or equal to the number of input meshes.
::	 If the number of output meshes is one then only the first mesh in the input list is saved.
::	 The format of the output mesh is guessed by the used extension.
::	 Script is optional and must be in the format saved by MeshLab.
