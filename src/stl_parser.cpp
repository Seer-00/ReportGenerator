#include "stl_parser.h"

extern JSONParser config;
extern Labels labels;
extern map<string, string> generated_image_map;

STLParser::STLParser() { }

STLParser::~STLParser() { }

STLParser* STLParser::get_instance()
{
	static STLParser instance;
	return &instance;
}

void STLParser::load_stl_files()
{
	/* 根据 config.json 文件中的 enabled_models，
	/* 结合 model_info.xml 中对应的属性，加载 STL 文件。
	/* 额外工作：
		1. 生成“颜色标示”所需的纯色 PNG 文件
		2. 将读入数据保存至 polydata 并设置颜色，
		   再存入 append_polydata，为后续生成 PLY 文件做准备
	*/

	if (has_read_stl) { return; }
	else { has_read_stl = true; }

	// 【额外工作】预生成 纯白色 PNG
	Value cfg = config.get_root();
	double white[3] = { 1.0, 1.0, 1.0 };
	const string out  = utf8_to_str(cfg["output_dir"].asString());
	const string name = LABEL_IMAGE_WHITE;
	write_label_image(LABEL_IMAGE_PATH(out, name), white);

	// 加载 STL 文件
	ModelInfoParser* p_model_info = ModelInfoParser::get_instance();
	Value enmodels = config.get_root()["enabled_models"];
	string type, path;
	for (string name : enmodels.getMemberNames()) {     // 可能含中文，要 utf8_to_str 转码
		Value model = enmodels[name];
		name = utf8_to_str(name);						// e.g. 右肺上叶
		type = utf8_to_str(model["type"].asString());	// e.g. stl
		path = utf8_to_str(model["path"].asString());	// e.g. ./input/model/右肺上叶.stl

		xml_node model_node = p_model_info->get_model_node(name, type);
		double color[3];
		p_model_info->get_color(model_node, color, 3);
		const bool visible      = p_model_info->get_visible(model_node);
		const double opacity    = p_model_info->get_opacity(model_node);
		const string organ_name = p_model_info->get_organ_name(model_node);

		vtkNew<vtkSTLReader> reader;
		reader->SetFileName(path.c_str());
		reader->Update();

		// 【额外工作】生成 纯色 PNG，并更新 labels
		if (visible) {
			write_label_image(LABEL_IMAGE_PATH(out, name), color);
			labels.name_map.insert(pair<int, string>(labels.idx++, name));
		}

		// 【额外工作】为后续生成 PLY 文件做准备
		vtkSmartPointer<vtkPolyData> polydata = reader->GetOutput();
		vtkNew<vtkUnsignedCharArray> colors;
		colors->SetName("Colors");
		colors->SetNumberOfComponents(3);
		colors->SetNumberOfTuples(polydata->GetNumberOfCells());
		double rgb[3];
		for (int i = 0; i < 3; i++) { rgb[i] = color[i] * 255.0; }
		for (int i = 0; i < polydata->GetNumberOfCells(); i++) {
			colors->InsertTuple(i, rgb);
		}
		polydata->GetCellData()->SetScalars(colors);

		append_polydata->AddInputData(polydata);
		append_polydata->Update();

		// 可视化
		vtkNew<vtkPolyDataMapper> mapper;
		mapper->SetInputConnection(reader->GetOutputPort());

		vtkNew<vtkActor> actor;
		actor->SetMapper(mapper);
		actor->SetVisibility(visible); // 可见性
		actor->GetProperty()->SetOpacity(opacity); // 透明度
		actor->GetProperty()->SetColor(color[0], color[1], color[2]); // RGB颜色
		renderer->AddActor(actor);
	}
	renderer->ResetCamera();
}

void STLParser::show_models()
{
	vtkNew<vtkRenderWindow> ren_win; // 用于预览模型的窗口
	vtkNew<vtkRenderWindowInteractor> renWinInteractor;
	ren_win->AddRenderer(renderer);		        // 关联
	renWinInteractor->SetRenderWindow(ren_win); // 关联
	renderer->ResetCamera();
	renderer->SetBackground(1.0, 1.0, 1.0);
	ren_win->SetSize(400, 400);
	ren_win->SetWindowName("STL模型预览");
	ren_win->Render(); // 渲染窗口
	renWinInteractor->Start();
}

void STLParser::capture_views()
{
	/* 根据 config.json 文件中的 enabled_views，
	/* 调整相机角度，并保存相应格式的图片文件。
	/* 读入数据异常时，这里采用默认值，当然也可以按需抛异常。*/
	/* [!] 调用该方法前，必须先调用 load_stl_files() 以加载模型文件 */

	vtkNew<vtkRenderWindow> ren_win;		// 用于截图的窗口
	ren_win->SetSize(400, 400);				// 默认窗口大小
	ren_win->SetOffScreenRendering(true);	// 不显示窗口
	ren_win->AddRenderer(renderer);			// 关联

	ModelInfoParser* p_model_info = ModelInfoParser::get_instance();
	Value enviews = config.get_root()["enabled_views"];
	string type;
	// 可能含中文，要 utf8_to_str 转码
	for (string name : enviews.getMemberNames()) {
		Value view = enviews[name];
		name = utf8_to_str(name); // e.g. 概览图
		type = utf8_to_str(view["type"].asString());  // e.g. png
		const int width    = view["width"].asInt();   // e.g. 800
		const int height   = view["height"].asInt();  // e.g. 800
		const int scale    = view["scale"].asInt();	  // e.g. 1
		const double zoom  = view["zoom"].asDouble(); // e.g. 1.0
		double bgc[3]; // e.g. [1.0, 1.0, 1.0]
		bgc[0] = bgc[1] = bgc[2] = 1.0; // 默认
		Value bgcs = view["background_color"];
		if (bgcs.isArray() && bgcs.size() == 3) {
			for (unsigned int i = 0; i < bgcs.size(); i++) {
				bgc[i] = bgcs[i].asDouble();
			}
		}
		// 设置背景颜色
		renderer->SetBackground(bgc[0], bgc[1], bgc[2]);
		// 备份原相机
		vtkNew<vtkCamera> origin_camera;
		origin_camera->DeepCopy(renderer->GetActiveCamera());
		// 调整用于截图的相机
		Value rotation = view["rotation"]; // e.g. ["Azimuth 10.0", "Roll 45.0"]
		vector<string> vec_rot;	           // e.g. ["Azimuth", "10.0"]
		for (unsigned int i = 0; i < rotation.size(); i++) { // 按顺序，解析多次旋转
			vec_rot = split_string(rotation[i].asString(), ' ');
			string rot_mode  = vec_rot[0];			   // 旋转模式
			double rot_angle = str2double(vec_rot[1]); // 旋转角度
			if		(rot_mode == "Roll")      { renderer->GetActiveCamera()->Roll(rot_angle); }
			else if (rot_mode == "Azimuth")   { renderer->GetActiveCamera()->Azimuth(rot_angle); }
			else if (rot_mode == "Elevation") { renderer->GetActiveCamera()->Elevation(rot_angle); }
			else { } // 默认
		}
		// 相机缩放
		renderer->GetActiveCamera()->Zoom(zoom);
		// 窗口大小（图片大小）
		ren_win->SetSize(width, height);
		// 渲染窗口
		ren_win->Render();
		// 捕获截图
		write_image(name, type, scale, ren_win);
		// 恢复原相机
		renderer->SetActiveCamera(origin_camera);
	}
}

void STLParser::write_image(string const& name, string const& type, const int& scale, vtkRenderWindow* renWin, bool rgba)
{
	/* 捕获当前窗口图像，保存至指定的文件和类型 */

	if (name.empty()) return;

	auto writer = vtkSmartPointer<vtkImageWriter>::New();
	if      (type == "bmp")  { writer = vtkSmartPointer<vtkBMPWriter>::New(); }
	else if (type == "jpg")  { writer = vtkSmartPointer<vtkJPEGWriter>::New(); }
	else if (type == "tiff") { writer = vtkSmartPointer<vtkTIFFWriter>::New(); }
	else                     { writer = vtkSmartPointer<vtkPNGWriter>::New(); } // 默认值

	vtkNew<vtkWindowToImageFilter> window_to_image_filter;
	if (rgba) { window_to_image_filter->SetInputBufferTypeToRGBA(); } // 额外记录alpha频道（透明度）的值
	else      { window_to_image_filter->SetInputBufferTypeToRGB(); }

	window_to_image_filter->SetInput(renWin);
	window_to_image_filter->SetScale(scale); // 图像质量
	window_to_image_filter->ReadFrontBufferOff();
	window_to_image_filter->Update();

	Value cfg = config.get_root();
	const string out_dir  = utf8_to_str(cfg["output_dir"].asString());
	const string filename = out_dir + "/" + name + "." + type;
	writer->SetFileName(filename.c_str());
	writer->SetInputConnection(window_to_image_filter->GetOutputPort());
	writer->Write();

	generated_image_map.insert(pair<string, string>(name, filename)); // 生成 PDF 时可能用到
}

void STLParser::write_label_image(string const& name, const double rgb[3])
{
	vtkNew<vtkImageCanvasSource2D> img_source;
	img_source->SetExtent(0, LABEL_W, 0, LABEL_H, 0, 0); // 设置画布三个维度上的最大最小值
	img_source->SetScalarTypeToUnsignedChar();  // 设置标量数据类型
	img_source->SetNumberOfScalarComponents(3); // 设置标量数据维度
	img_source->SetDrawColor(rgb[0] * 255, rgb[1] * 255, rgb[2] * 255);
	img_source->FillBox(0, 22, 0, 10);
	img_source->Update();

	vtkNew<vtkImageCast> cast_filter;
	cast_filter->SetOutputScalarTypeToUnsignedChar();
	cast_filter->SetInputConnection(img_source->GetOutputPort());
	cast_filter->Update();

	vtkNew<vtkPNGWriter> writer;
	writer->SetFileName(name.c_str());
	writer->SetInputConnection(cast_filter->GetOutputPort());
	writer->Write();
}

void STLParser::write_ply()
{
	Value cfg = config.get_root();
	const string out_dir  = utf8_to_str(cfg["output_dir"].asString());
	const string filename = PLY_MODEL_PATH(out_dir);

	vtkNew<vtkPLYWriter> plyWriter;
	plyWriter->SetFileName(filename.c_str());
	plyWriter->SetArrayName("Colors");
	plyWriter->SetInputConnection(append_polydata->GetOutputPort());
	plyWriter->Write();
}
