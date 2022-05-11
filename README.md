## ReportGenerator

|                           |                                                            |
| ------------------------- | ---------------------------------------------------------- |
| **开发者**                | Seer                                                       |
| **最后更新日期**          | 2022-05-11                                                 |
| **最新版本和地址（dev）** | [v1.0] https://github.com/Seer-00/ReportGenerator/tree/dev |

* 主要功能
	
	* 编辑**配置文件**，输入**报告模板、填充内容、模型文件、模型信息**，输出**模型视图、模型PLY/U3D文件、PDF报告**
	
* 特色
	* 可以定义**默认填充内容**，作为**填充内容**的默认值
	* 对于填充内容，可选择用户输入、默认输入、软件生成三种方式之一，分别需要在**填充内容、默认填充内容**、代码层面进行修改
	* 可以修改和自定义**报告模板**，包括增删元素、修改元素的大小、位置等
	* 可以修改和自定义**填充内容**，包括文本和图片
	* 对于图片，可以修改来源、位置、大小等；对于文本，可以修改内容、位置、字体、样式等
	* 可以修改**配置文件**，指定输入/输出目录、启用模型文件、启用报告页码等
	* 输入文件之间耦合度低，在具有多个同类型文件时，可按需组合使用

#### 项目结构与文件说明（以 **"." **表示本REAMDE.md文件所在目录）

| 文件（夹）                               | 说明                                                         |
| ---------------------------------------- | ------------------------------------------------------------ |
| ./release/baidupan.txt                   | VS 2019 Debug 版本的发布。**通过百度网盘链接下载即可**。     |
| ./bin/vtk                                | vtk-8.2 使用的 \*.dll                                        |
| ./bin/pdflib                             | PDFLib 使用的 \*.dll                                         |
| ./lib/vtk                                | vtk-8.2 使用的 \*.lib                                        |
| ./lib/pdflib                             | PDFLib 使用的 \*.lib                                         |
| ./include                                | 本项目源码的 \*.h 文件，包括第三方库                         |
| ./src                                    | 本项目源码的 \*.cpp 文件，包括第三方库                       |
| ./ui                                     | 本项目图形化界面的 \*.ui 文件，由 QTDesigner 生成，ui_\*.h 由 QT 的 uic 工具生成 |
| ./config.json                            | 作为软件的**配置文件**                                       |
| ./input/model                            | 作为软件的输入，是STL模型；其中 model_info.xml 包含了**模型信息** |
| ./input/input_default.json               | 作为软件的输入，包含默认的输入信息（**填充内容的默认值**）   |
| ./input/input_user.json                  | 作为软件的输入，包含用户的输入信息（**填充内容**）           |
| ./input/report_template.xml              | 作为软件的输入，是待生成报告的模板（**报告模板**）           |
| ./resource/demo                          | 作为演示资源，可在**配置文件**中将 "res_input_user" 的值设置为 input_demo.json |
| ./resource/font                          | 作为字体资源，可扩展                                         |
| ./resource/supplement/MeshLab*.exe       | 作为软件的补充，安装 MeshLab 后，可借助 bat 脚本转换 \*.u3d 或 手动生成 \*.u3d |
| ./resource/supplement/PLY_TO_U3D.bat     | 作为软件的补充，是 PLY -> U3D 的 bat 脚本，注意编辑其中 MeshLabSever 的路径 |
| ./resource/supplement/Simplification.mlx | 作为软件的补充，执行上述bat脚本时，可调用的MeshLab脚本，用于简化3D模型 |
| ./resource/supplement/STL_TO_U3D.mlab    | 作为软件的补充，是 MeVisLab 的工程文件，可用于手动生成 \*.u3d |
| ./resource/inputflags.json               | 作为资源，在软件运行过程中，提供 模板 映射到 用户输入 的**对应规则** |
| ./resource/logo.png                      | PDF报告 page 1 的 logo                                       |
| ./output                                 | 作为软件的工作和输出目录，可以在配置文件中更改 "output_dir"  |

#### 第三方库与来源

| 第三方库 | 版本   | 来源                                           | 说明                                      |
| -------- | ------ | ---------------------------------------------- | ----------------------------------------- |
| PDFlib   | 10.0.0 | https://www.pdflib.com/                        | 生成 PDF                                  |
| VTK      | 8.2    | https://vtk.org/                               | 解析、渲染 STL 文件，生成视图、PLY 文件等 |
| pugixml  | 1.12   | https://pugixml.org/                           | 解析 XML 文件                             |
| jsoncpp  | 1.8.0  | https://github.com/open-source-parsers/jsoncpp | 解析 JSON 文件                            |

#### 补充软件与来源

| 软件     | 版本              | 来源                     |
| -------- | ----------------- | ------------------------ |
| MeshLab  | 2020.07 (windows) | https://www.meshlab.net/ |
| MeVisLab | 3.4.3 (vs2017-64) | https://www.mevislab.de/ |

* 说明
	* MeshLab：可用于手动创建 \*.u3d 模型文件，也可在本软件中调用 bat 脚本自动转换得到 \*.u3d 文件。
		* 具体方法可参考：https://blog.csdn.net/sxf1061926959/article/details/53869429
	* MeVisLab：可用于手动创建 \*.u3d 模型文件，比 MeshLab 的效果好。
		* 具体方法可参考：Newe A, Ganslandt T (2013) Simplified Generation of Biomedical 3D Surface Model Data for Embedding into 3D Portable Document Format (PDF) Files for Publication and Education. PLoS ONE 8(11): e79004.     https://doi.org/10.1371/journal.pone.0079004