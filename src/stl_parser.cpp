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
	/* ���� config.json �ļ��е� enabled_models��
	/* ��� model_info.xml �ж�Ӧ�����ԣ����� STL �ļ���
	/* ���⹤����
		1. ���ɡ���ɫ��ʾ������Ĵ�ɫ PNG �ļ�
		2. ���������ݱ����� polydata ��������ɫ��
		   �ٴ��� append_polydata��Ϊ�������� PLY �ļ���׼��
	*/

	if (has_read_stl) { return; }
	else { has_read_stl = true; }

	// �����⹤����Ԥ���� ����ɫ PNG
	Value cfg = config.get_root();
	double white[3] = { 1.0, 1.0, 1.0 };
	const string out  = utf8_to_str(cfg["output_dir"].asString());
	const string name = LABEL_IMAGE_WHITE;
	write_label_image(LABEL_IMAGE_PATH(out, name), white);

	// ���� STL �ļ�
	ModelInfoParser* p_model_info = ModelInfoParser::get_instance();
	Value enmodels = config.get_root()["enabled_models"];
	string type, path;
	for (string name : enmodels.getMemberNames()) {     // ���ܺ����ģ�Ҫ utf8_to_str ת��
		Value model = enmodels[name];
		name = utf8_to_str(name);						// e.g. �ҷ���Ҷ
		type = utf8_to_str(model["type"].asString());	// e.g. stl
		path = utf8_to_str(model["path"].asString());	// e.g. ./input/model/�ҷ���Ҷ.stl

		xml_node model_node = p_model_info->get_model_node(name, type);
		double color[3];
		p_model_info->get_color(model_node, color, 3);
		const bool visible      = p_model_info->get_visible(model_node);
		const double opacity    = p_model_info->get_opacity(model_node);
		const string organ_name = p_model_info->get_organ_name(model_node);

		vtkNew<vtkSTLReader> reader;
		reader->SetFileName(path.c_str());
		reader->Update();

		// �����⹤�������� ��ɫ PNG�������� labels
		if (visible) {
			write_label_image(LABEL_IMAGE_PATH(out, name), color);
			labels.name_map.insert(pair<int, string>(labels.idx++, name));
		}

		// �����⹤����Ϊ�������� PLY �ļ���׼��
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

		// ���ӻ�
		vtkNew<vtkPolyDataMapper> mapper;
		mapper->SetInputConnection(reader->GetOutputPort());

		vtkNew<vtkActor> actor;
		actor->SetMapper(mapper);
		actor->SetVisibility(visible); // �ɼ���
		actor->GetProperty()->SetOpacity(opacity); // ͸����
		actor->GetProperty()->SetColor(color[0], color[1], color[2]); // RGB��ɫ
		renderer->AddActor(actor);
	}
	renderer->ResetCamera();
}

void STLParser::show_models()
{
	vtkNew<vtkRenderWindow> ren_win; // ����Ԥ��ģ�͵Ĵ���
	vtkNew<vtkRenderWindowInteractor> renWinInteractor;
	ren_win->AddRenderer(renderer);		        // ����
	renWinInteractor->SetRenderWindow(ren_win); // ����
	renderer->ResetCamera();
	renderer->SetBackground(1.0, 1.0, 1.0);
	ren_win->SetSize(400, 400);
	ren_win->SetWindowName("STLģ��Ԥ��");
	ren_win->Render(); // ��Ⱦ����
	renWinInteractor->Start();
}

void STLParser::capture_views()
{
	/* ���� config.json �ļ��е� enabled_views��
	/* ��������Ƕȣ���������Ӧ��ʽ��ͼƬ�ļ���
	/* ���������쳣ʱ���������Ĭ��ֵ����ȻҲ���԰������쳣��*/
	/* [!] ���ø÷���ǰ�������ȵ��� load_stl_files() �Լ���ģ���ļ� */

	vtkNew<vtkRenderWindow> ren_win;		// ���ڽ�ͼ�Ĵ���
	ren_win->SetSize(400, 400);				// Ĭ�ϴ��ڴ�С
	ren_win->SetOffScreenRendering(true);	// ����ʾ����
	ren_win->AddRenderer(renderer);			// ����

	ModelInfoParser* p_model_info = ModelInfoParser::get_instance();
	Value enviews = config.get_root()["enabled_views"];
	string type;
	// ���ܺ����ģ�Ҫ utf8_to_str ת��
	for (string name : enviews.getMemberNames()) {
		Value view = enviews[name];
		name = utf8_to_str(name); // e.g. ����ͼ
		type = utf8_to_str(view["type"].asString());  // e.g. png
		const int width    = view["width"].asInt();   // e.g. 800
		const int height   = view["height"].asInt();  // e.g. 800
		const int scale    = view["scale"].asInt();	  // e.g. 1
		const double zoom  = view["zoom"].asDouble(); // e.g. 1.0
		double bgc[3]; // e.g. [1.0, 1.0, 1.0]
		bgc[0] = bgc[1] = bgc[2] = 1.0; // Ĭ��
		Value bgcs = view["background_color"];
		if (bgcs.isArray() && bgcs.size() == 3) {
			for (unsigned int i = 0; i < bgcs.size(); i++) {
				bgc[i] = bgcs[i].asDouble();
			}
		}
		// ���ñ�����ɫ
		renderer->SetBackground(bgc[0], bgc[1], bgc[2]);
		// ����ԭ���
		vtkNew<vtkCamera> origin_camera;
		origin_camera->DeepCopy(renderer->GetActiveCamera());
		// �������ڽ�ͼ�����
		Value rotation = view["rotation"]; // e.g. ["Azimuth 10.0", "Roll 45.0"]
		vector<string> vec_rot;	           // e.g. ["Azimuth", "10.0"]
		for (unsigned int i = 0; i < rotation.size(); i++) { // ��˳�򣬽��������ת
			vec_rot = split_string(rotation[i].asString(), ' ');
			string rot_mode  = vec_rot[0];			   // ��תģʽ
			double rot_angle = str2double(vec_rot[1]); // ��ת�Ƕ�
			if		(rot_mode == "Roll")      { renderer->GetActiveCamera()->Roll(rot_angle); }
			else if (rot_mode == "Azimuth")   { renderer->GetActiveCamera()->Azimuth(rot_angle); }
			else if (rot_mode == "Elevation") { renderer->GetActiveCamera()->Elevation(rot_angle); }
			else { } // Ĭ��
		}
		// �������
		renderer->GetActiveCamera()->Zoom(zoom);
		// ���ڴ�С��ͼƬ��С��
		ren_win->SetSize(width, height);
		// ��Ⱦ����
		ren_win->Render();
		// �����ͼ
		write_image(name, type, scale, ren_win);
		// �ָ�ԭ���
		renderer->SetActiveCamera(origin_camera);
	}
}

void STLParser::write_image(string const& name, string const& type, const int& scale, vtkRenderWindow* renWin, bool rgba)
{
	/* ����ǰ����ͼ�񣬱�����ָ�����ļ������� */

	if (name.empty()) return;

	auto writer = vtkSmartPointer<vtkImageWriter>::New();
	if      (type == "bmp")  { writer = vtkSmartPointer<vtkBMPWriter>::New(); }
	else if (type == "jpg")  { writer = vtkSmartPointer<vtkJPEGWriter>::New(); }
	else if (type == "tiff") { writer = vtkSmartPointer<vtkTIFFWriter>::New(); }
	else                     { writer = vtkSmartPointer<vtkPNGWriter>::New(); } // Ĭ��ֵ

	vtkNew<vtkWindowToImageFilter> window_to_image_filter;
	if (rgba) { window_to_image_filter->SetInputBufferTypeToRGBA(); } // �����¼alphaƵ����͸���ȣ���ֵ
	else      { window_to_image_filter->SetInputBufferTypeToRGB(); }

	window_to_image_filter->SetInput(renWin);
	window_to_image_filter->SetScale(scale); // ͼ������
	window_to_image_filter->ReadFrontBufferOff();
	window_to_image_filter->Update();

	Value cfg = config.get_root();
	const string out_dir  = utf8_to_str(cfg["output_dir"].asString());
	const string filename = out_dir + "/" + name + "." + type;
	writer->SetFileName(filename.c_str());
	writer->SetInputConnection(window_to_image_filter->GetOutputPort());
	writer->Write();

	generated_image_map.insert(pair<string, string>(name, filename)); // ���� PDF ʱ�����õ�
}

void STLParser::write_label_image(string const& name, const double rgb[3])
{
	vtkNew<vtkImageCanvasSource2D> img_source;
	img_source->SetExtent(0, LABEL_W, 0, LABEL_H, 0, 0); // ���û�������ά���ϵ������Сֵ
	img_source->SetScalarTypeToUnsignedChar();  // ���ñ�����������
	img_source->SetNumberOfScalarComponents(3); // ���ñ�������ά��
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
