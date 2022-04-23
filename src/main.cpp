#include "my_pdflib.h"

using namespace std;
using namespace pdflib;

const wstring outfile = L"./temp/_output.pdf";

const wstring info_creator = L"Report Generator";
const wstring info_title = L"3D 建模可视化报告";

const wstring res_u3d = L"u3d_demo.u3d";

const wstring res_page1_img1 = L"page_1_img_1_demo.png";
const wstring res_page2_img1 = L"page_2_img_1_demo.png";
const wstring res_page3_img1 = L"page_3_img_1_demo.png";
const wstring res_page3_img2 = L"page_3_img_2_demo.png";
const wstring res_page3_img3 = L"page_3_img_3_demo.png";
const wstring res_page3_img4 = L"page_3_img_4_demo.png";
const wstring res_page4_img1 = L"page_4_img_1_demo.png";
const wstring res_page4_img2 = L"page_4_img_2_demo.png";

const wstring res_font_calibri = L"calibri.ttf";
const wstring res_font_msyh	   = L"MicrosoftYaHei.ttf";
const wstring res_font_msyhbd  = L"MicrosoftYaHei-Bold.ttf";
const wstring res_font_simsun  = L"SimSun.ttf";

MyPDFlib pdf;

int font_calibri;
int font_msyh;
int font_msyhbd;
int font_simsun;

void set_options()
{
	wstring optlist = L"";
	// errorpolicy: 必须检查 load_xxx() 等函数的返回值
	optlist.append(L"errorpolicy=return ");
	// searchpath: 资源搜索路径
	optlist.append(L"searchpath=");
	optlist.append(L"{");
	optlist.append(L"{./resource} ");
	optlist.append(L"{./resource/font} ");
	optlist.append(L"{./resource/res_demo} ");
	optlist.append(L"} ");
	// fontoutline: 字体配置
	optlist.append(L"FontOutline=");
	optlist.append(L"{");
	optlist.append(L"calibri=" + res_font_calibri + L" ");	// Calibri
	optlist.append(L"msyh=" + res_font_msyh + L" ");		// 微软雅黑
	optlist.append(L"msyhbd=" + res_font_msyhbd + L" ");	// 微软雅黑粗体
	optlist.append(L"simsun=" + res_font_simsun + L" ");	// 宋体
	optlist.append(L"} ");

	pdf.get_p().set_option(optlist);
}

void set_infos()
{
	pdf.get_p().set_info(L"Creator", info_creator);
	pdf.get_p().set_info(L"Title", info_title);
}

void load_fonts()
{
	font_calibri = pdf.load_font(L"calibri", L"unicode", L"");
	font_msyh = pdf.load_font(L"msyh", L"unicode", L"");
	font_msyhbd = pdf.load_font(L"msyhbd", L"unicode", L"");
	font_simsun = pdf.load_font(L"simsun", L"unicode", L"");
}

void init()
{
	set_options();

	set_infos();

	//load_fonts();
}

void generate_page1()
{
	int tf = -1;
	wstring optlist;
	wstring result;

	pdf.get_p().begin_page_ext(a4_width, a4_height, L"topdown=true");

	int page_1_img_1 = pdf.load_image(L"png", res_page1_img1, L""); // 相比于auto，显式指定png可以略微提升性能
	pdf.get_p().fit_image( /* 图像句柄，横坐标，纵坐标，选项列表 */
		page_1_img_1,
		a4_width / 2, // 居中
		129, // 根据模板微调
		L"position={center}" // 以图像中心为基准点
	);
	pdf.get_p().close_image(page_1_img_1);

	optlist = L"fontname=calibri fontsize=20 encoding=unicode alignment=center fillcolor={rgb 0.180392 0.454901 0.709803}";
	tf = pdf.add_textflow(tf, L"3D ", optlist);
	optlist = L"fontname=simsun fontsize=20 encoding=unicode alignment=center fillcolor={rgb 0.180392 0.454901 0.709803}";
	tf = pdf.add_textflow(tf, L"建模可视化报告", optlist);
	result = pdf.fit_textflow(tf, 0, 208, a4_width, 240, L"verticalalign={center}");
	pdf.get_p().delete_textflow(tf);

	optlist = L"fontname=msyh fontsize=10.5 encoding=unicode position={center} fillcolor={rgb 0.0 0.439215 0.752941}";
	pdf.get_p().fit_textline(L"编号： 20210818-XXXX-0002", a4_width / 2, 269, optlist);

	optlist = L"fontname=simsun fontsize=14 encoding=unicode position={center} fillcolor={rgb 0.356862 0.607843 0.835294}";
	pdf.get_p().fit_textline(L"可自定义的主体名称", a4_width / 2, 649, optlist);

	vector<wstring> ymd;
	fill_today_date(ymd); // [year, month, day]
	optlist = L"fontsize=14 encoding=unicode alignment=center fillcolor={rgb 0.356862 0.607843 0.835294}";
	tf = -1;
	tf = pdf.add_textflow(tf, ymd[0], L"fontname=calibri " + optlist);
	tf = pdf.add_textflow(tf, L" 年 ", L"fontname=simsun " + optlist);
	tf = pdf.add_textflow(tf, ymd[1], L"fontname=calibri " + optlist);
	tf = pdf.add_textflow(tf, L" 月 ", L"fontname=simsun " + optlist);
	tf = pdf.add_textflow(tf, ymd[2], L"fontname=calibri " + optlist);
	tf = pdf.add_textflow(tf, L" 日", L"fontname=simsun " + optlist);
	result = pdf.fit_textflow(tf, 0, 670, a4_width, 685, L"verticalalign={center}");
	pdf.get_p().delete_textflow(tf);

	optlist = L"fontname=simsun fontsize=10 encoding=unicode position={center}";
	pdf.get_p().fit_textline(L"1", a4_width / 2, 787, optlist);

	pdf.get_p().end_page_ext(L"");
}

void generate_page2()
{
	int tbl = -1;
	wstring optlist;
	wstring result;

	pdf.get_p().begin_page_ext(a4_width, a4_height, L"topdown=true");

	optlist = L"fontname=msyhbd fontsize=18 encoding=unicode position={center}";
	pdf.get_p().fit_textline(L"3D 建模可视化报告", a4_width / 2, 72, optlist);
	
	// table 1
	optlist = L"fittextline={position={right center} fontname=msyhbd fontsize=12} colwidth=72 rowheight=23 margin=6";
	tbl = pdf.add_table_cell(tbl, 1, 1, L"编\u3000号：", optlist);
	tbl = pdf.add_table_cell(tbl, 1, 2, L"姓\u3000名：", optlist);
	tbl = pdf.add_table_cell(tbl, 1, 3, L"性\u3000别：", optlist);

	optlist = L"fittextline={position={left center} fontname=msyh fontsize=10.5} colwidth=165 rowheight=23 margin=6";
	tbl = pdf.add_table_cell(tbl, 2, 1, L"20210818-XXXX-0001", optlist);
	tbl = pdf.add_table_cell(tbl, 2, 2, L"XXX", optlist);
	tbl = pdf.add_table_cell(tbl, 2, 3, L"女", optlist);
	
	optlist = L"fittextline={position={right center} fontname=msyhbd fontsize=12} colwidth=92 rowheight=23 margin=6";
	tbl = pdf.add_table_cell(tbl, 3, 1, L"建模日期：", optlist);
	tbl = pdf.add_table_cell(tbl, 3, 2, L"医\u3000\u3000院：", optlist);
	tbl = pdf.add_table_cell(tbl, 3, 3, L"需求科室：", optlist);

	optlist = L"fittextline={position={left center} fontname=msyh fontsize=10.5} colwidth=154 rowheight=23 margin=6";
	tbl = pdf.add_table_cell(tbl, 4, 1, L"20210818", optlist);
	tbl = pdf.add_table_cell(tbl, 4, 2, L"XXXXX 医院", optlist);
	tbl = pdf.add_table_cell(tbl, 4, 3, L"胸外科", optlist);
	
	optlist = L"stroke={{line=frame linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}} {line=other linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}}}";
	result = pdf.fit_table(tbl, 56.5, 119, 538.5, 188, optlist);
	pdf.get_p().delete_table(tbl, L""); // also delete textflow, image, etc.

	// table 2
	tbl = -1;

	int page_2_img_1 = pdf.load_image(L"png", res_page2_img1, L"");
	optlist = L"image=" + int_to_wstr(page_2_img_1) + L" colwidth=482 rowheight=412 colspan=8 margin=4";	
	tbl = pdf.add_table_cell(tbl, 1, 1, L"", optlist);

	optlist = L"fittextline={position={left center} fontname=msyhbd fontsize=11} rowheight=32 colspan=8 margin=6";
	tbl = pdf.add_table_cell(tbl, 1, 2, L"颜色标示：", optlist);
	
	// group 1
	optlist = L"fittextline={position={left center} fontname=msyh fontsize=11} colwidth=38.5 rowheight=32 margin=6";
	tbl = pdf.add_table_cell(tbl, 1, 3, L"x", optlist);
	tbl = pdf.add_table_cell(tbl, 1, 4, L"x", optlist);
	tbl = pdf.add_table_cell(tbl, 1, 5, L"?", optlist);

	optlist = L"fittextline={position={left center} fontname=msyh fontsize=11} colwidth=82 rowheight=32 margin=6";
	tbl = pdf.add_table_cell(tbl, 2, 3, L"肺静脉血管", optlist);
	tbl = pdf.add_table_cell(tbl, 2, 4, L"肺", optlist);
	tbl = pdf.add_table_cell(tbl, 2, 5, L"?", optlist);
	
	// group 2
	optlist = L"fittextline={position={left center} fontname=msyh fontsize=11} colwidth=38.5 rowheight=32 margin=6";
	tbl = pdf.add_table_cell(tbl, 3, 3, L"x", optlist);
	tbl = pdf.add_table_cell(tbl, 3, 4, L"?", optlist);
	tbl = pdf.add_table_cell(tbl, 3, 5, L"?", optlist);

	optlist = L"fittextline={position={left center} fontname=msyh fontsize=11} colwidth=82 rowheight=32 margin=6";
	tbl = pdf.add_table_cell(tbl, 4, 3, L"肺动脉血管", optlist);
	tbl = pdf.add_table_cell(tbl, 4, 4, L"?", optlist);
	tbl = pdf.add_table_cell(tbl, 4, 5, L"?", optlist);
	
	// group 3
	optlist = L"fittextline={position={left center} fontname=msyh fontsize=11} colwidth=38.5 rowheight=32 margin=6";
	tbl = pdf.add_table_cell(tbl, 5, 3, L"x", optlist);
	tbl = pdf.add_table_cell(tbl, 5, 4, L"?", optlist);
	tbl = pdf.add_table_cell(tbl, 5, 5, L"?", optlist);

	optlist = L"fittextline={position={left center} fontname=msyh fontsize=11} colwidth=82 rowheight=32 margin=6";
	tbl = pdf.add_table_cell(tbl, 6, 3, L"支气管", optlist);
	tbl = pdf.add_table_cell(tbl, 6, 4, L"?", optlist);
	tbl = pdf.add_table_cell(tbl, 6, 5, L"?", optlist);
	
	// group 4
	optlist = L"fittextline={position={left center} fontname=msyh fontsize=11} colwidth=38.5 rowheight=32 margin=6";
	tbl = pdf.add_table_cell(tbl, 7, 3, L"x", optlist);
	tbl = pdf.add_table_cell(tbl, 7, 4, L"?", optlist);
	tbl = pdf.add_table_cell(tbl, 7, 5, L"?", optlist);

	optlist = L"fittextline={position={left center} fontname=msyh fontsize=11} colwidth=82 rowheight=32 margin=6";
	tbl = pdf.add_table_cell(tbl, 8, 3, L"占位", optlist);
	tbl = pdf.add_table_cell(tbl, 8, 4, L"?", optlist);
	tbl = pdf.add_table_cell(tbl, 8, 5, L"?", optlist);

	// rgb 189 214 238
	optlist = L"stroke={{line=frame linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}} {line=other linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}}}";
	result = pdf.fit_table(tbl, 56.5, 188, 538.5, 820, optlist);
	pdf.get_p().delete_table(tbl, L"");

	optlist = L"fontname=simsun fontsize=10 encoding=unicode position={center}";
	pdf.get_p().fit_textline(L"2", a4_width / 2, 787, optlist);

	pdf.get_p().end_page_ext(L"");
}

void generate_page3()
{
	int tbl = -1;
	wstring optlist;
	wstring result;

	pdf.get_p().begin_page_ext(a4_width, a4_height, L"topdown=true");
	
	// table 1
	optlist = L"fittextline={position={center top} fontname=msyhbd fontsize=12} colwidth=482 rowheight=42 margin=12";
	tbl = pdf.add_table_cell(tbl, 1, 1, L"空间位置关系图", optlist);

	optlist = L"stroke={{line=frame linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}} {line=other linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}}}";
	result = pdf.fit_table(tbl, 56.5, 57, 538.5, 99, optlist);
	pdf.get_p().delete_table(tbl, L"");

	// table 2
	tbl = -1;
	optlist = L"fittextline={position={left center} fontname=msyh fontsize=10.5} colwidth=241 rowheight=23 margin=6";
	tbl = pdf.add_table_cell(tbl, 1, 1, L"上面观：", optlist);
	tbl = pdf.add_table_cell(tbl, 2, 1, L"下面观：", optlist);

	optlist = L"stroke={{line=frame linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}} {line=other linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}}}";
	result = pdf.fit_table(tbl, 56.5, 99, 538.5, 122, optlist);
	pdf.get_p().delete_table(tbl, L"");

	// table 3
	tbl = -1;
	optlist = L"fittextline={position={left center} fontname=msyh fontsize=10.5} colwidth=241 rowheight=23 margin=6";
	
	int page_3_img_1 = pdf.load_image(L"png", res_page3_img1, L"");
	optlist = L"image=" + int_to_wstr(page_3_img_1) + L" colwidth=241 rowheight=238 margin=4";
	tbl = pdf.add_table_cell(tbl, 1, 1, L"", optlist);

	int page_3_img_2 = pdf.load_image(L"png", res_page3_img2, L"");
	optlist = L"image=" + int_to_wstr(page_3_img_2) + L" colwidth=241 rowheight=238 margin=4";
	tbl = pdf.add_table_cell(tbl, 2, 1, L"", optlist);

	optlist = L"stroke={{line=frame linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}} {line=other linewidth=0}}";
	result = pdf.fit_table(tbl, 56.5, 122, 538.5, 360, optlist);
	pdf.get_p().delete_table(tbl, L"");

	// table 4
	tbl = -1;
	optlist = L"fittextline={position={left center} fontname=msyh fontsize=10.5} colwidth=241 rowheight=23 margin=6";
	tbl = pdf.add_table_cell(tbl, 1, 1, L"正面观：", optlist);
	tbl = pdf.add_table_cell(tbl, 2, 1, L"背面观：", optlist);

	optlist = L"stroke={{line=frame linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}} {line=other linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}}}";
	result = pdf.fit_table(tbl, 56.5, 360, 538.5, 383, optlist);
	pdf.get_p().delete_table(tbl, L"");

	// table 5
	tbl = -1;
	optlist = L"fittextline={position={left center} fontname=msyh fontsize=10.5} colwidth=241 rowheight=23 margin=6";

	int page_3_img_3 = pdf.load_image(L"png", res_page3_img3, L"");
	optlist = L"image=" + int_to_wstr(page_3_img_3) + L" colwidth=241 rowheight=330 margin=4";
	tbl = pdf.add_table_cell(tbl, 1, 1, L"", optlist);

	int page_3_img_4 = pdf.load_image(L"png", res_page3_img4, L"");
	optlist = L"image=" + int_to_wstr(page_3_img_4) + L" colwidth=241 rowheight=330 margin=4";
	tbl = pdf.add_table_cell(tbl, 2, 1, L"", optlist);

	optlist = L"stroke={{line=frame linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}} {line=other linewidth=0}}";
	result = pdf.fit_table(tbl, 56.5, 383, 538.5, 713, optlist);
	pdf.get_p().delete_table(tbl, L"");

	optlist = L"fontname=simsun fontsize=10 encoding=unicode position={center}";
	pdf.get_p().fit_textline(L"3", a4_width / 2, 787, optlist);

	pdf.get_p().end_page_ext(L"");
}

void generate_page4()
{
	int tbl = -1;
	wstring optlist;
	wstring result;

	pdf.get_p().begin_page_ext(a4_width, a4_height, L"topdown=true");

	// table 1
	optlist = L"fittextline={position={center top} fontname=msyhbd fontsize=12} colwidth=482 rowheight=42 margin=12";
	tbl = pdf.add_table_cell(tbl, 1, 1, L"空间位置关系图", optlist);

	optlist = L"stroke={{line=frame linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}} {line=other linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}}}";
	result = pdf.fit_table(tbl, 56.5, 57, 538.5, 99, optlist);
	pdf.get_p().delete_table(tbl, L"");

	// table 2
	tbl = -1;
	optlist = L"fittextline={position={left center} fontname=msyh fontsize=10.5} colwidth=241 rowheight=23 margin=6";
	tbl = pdf.add_table_cell(tbl, 1, 1, L"左侧面观：", optlist);
	tbl = pdf.add_table_cell(tbl, 2, 1, L"右侧面观：", optlist);

	optlist = L"stroke={{line=frame linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}} {line=other linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}}}";
	result = pdf.fit_table(tbl, 56.5, 99, 538.5, 122, optlist);
	pdf.get_p().delete_table(tbl, L"");

	// table 3
	tbl = -1;
	optlist = L"fittextline={position={left center} fontname=msyh fontsize=10.5} colwidth=241 rowheight=23 margin=6";

	int page_4_img_1 = pdf.load_image(L"png", res_page4_img1, L"");
	optlist = L"image=" + int_to_wstr(page_4_img_1) + L" colwidth=241 rowheight=428 margin=4";
	tbl = pdf.add_table_cell(tbl, 1, 1, L"", optlist);

	int page_4_img_2 = pdf.load_image(L"png", res_page4_img2, L"");
	optlist = L"image=" + int_to_wstr(page_4_img_2) + L" colwidth=241 rowheight=428 margin=4";
	tbl = pdf.add_table_cell(tbl, 2, 1, L"", optlist);

	optlist = L"stroke={{line=frame linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}} {line=other linewidth=0}}";
	result = pdf.fit_table(tbl, 56.5, 122, 538.5, 550, optlist);
	pdf.get_p().delete_table(tbl, L"");

	optlist = L"fontname=simsun fontsize=10 encoding=unicode position={center}";
	pdf.get_p().fit_textline(L"4", a4_width / 2, 787, optlist);

	pdf.get_p().end_page_ext(L"");
}

void generate_page5()
{
	wstring optlist;
	wstring result;

	pdf.get_p().begin_page_ext(a4_width, a4_height, L"topdown=true");

	optlist = L"type=U3D name=FirstView background={fillcolor=Lavender} camera2world={-1 0 0 0 0 1 0 1 0 0.5 -100 0.5}";
	int view = pdf.create_3dview(L"initial view", optlist);
	
	optlist = L"type=U3D views={" + int_to_wstr(view) + L"}";
	int data = pdf.load_3ddata(res_u3d, optlist);

	optlist = L"name=annot usercoordinates contents=U3D 3Ddata= " + int_to_wstr(data)
		+ L" 3Dactivate={enable=open} 3Dinitialview=" + int_to_wstr(view);
	pdf.get_p().create_annotation(116, 200, 447, 580, L"3D", optlist);

	pdf.get_p().end_page_ext(L"");
}

void generate_page6()
{
	int tbl = -1;
	wstring optlist;
	wstring result;

	pdf.get_p().begin_page_ext(a4_width, a4_height, L"topdown=true");

	// table 1
	optlist = L"fittextline={position={center top} fontname=msyhbd fontsize=12} colwidth=482 rowheight=73 colspan=4 margin=12";
	tbl = pdf.add_table_cell(tbl, 1, 1, L"原始CT DICOM数据分析", optlist);

	optlist = L"fittextline={position={center} fontname=msyhbd fontsize=10.5} colwidth=120.5 rowheight=39 margin=12";
	tbl = pdf.add_table_cell(tbl, 1, 2, L"数据扫描部位：", optlist);
	tbl = pdf.add_table_cell(tbl, 3, 2, L"图像来源设备：", optlist);

	optlist = L"fittextline={position={center} fontname=msyh fontsize=10.5} colwidth=120.5 rowheight=39 margin=12";
	tbl = pdf.add_table_cell(tbl, 2, 2, L"胸部", optlist);
	tbl = pdf.add_table_cell(tbl, 4, 2, L"CT", optlist);

	optlist = L"fittextline={position={center top} fontname=msyhbd fontsize=10.5} colwidth=120.5 rowheight=39 margin=6";
	tbl = pdf.add_table_cell(tbl, 1, 3, L"数据报告项", optlist);
	tbl = pdf.add_table_cell(tbl, 2, 3, L"文件数", optlist);

	optlist = L"fittextline={position={center top} fontname=msyhbd fontsize=10.5} colwidth=120.5 rowheight=39 colspan=2 margin=6";
	tbl = pdf.add_table_cell(tbl, 3, 3, L"备注", optlist);

	optlist = L"fittextline={position={left top} fontname=msyh fontsize=10.5} colwidth=120.5 rowheight=56 margin=6";
	tbl = pdf.add_table_cell(tbl, 1, 4, L"DICOM数据文件数量", optlist);
	tbl = pdf.add_table_cell(tbl, 1, 5, L"分期数量", optlist);
	tbl = pdf.add_table_cell(tbl, 1, 6, L"薄层扫描期", optlist);
	tbl = pdf.add_table_cell(tbl, 1, 7, L"原始CT诊断报告", optlist);

	optlist = L"fittextline={position={center top} fontname=msyh fontsize=10.5} colwidth=120.5 rowheight=56 margin=6";
	tbl = pdf.add_table_cell(tbl, 2, 4, L"577", optlist);
	tbl = pdf.add_table_cell(tbl, 2, 5, L"8", optlist);
	tbl = pdf.add_table_cell(tbl, 2, 6, L"1", optlist);
	tbl = pdf.add_table_cell(tbl, 2, 7, L"有", optlist);

	optlist = L"fittextline={position={left top} fontname=msyh fontsize=10.5} colwidth=120.5 rowheight=56 colspan=2 margin=6";
	tbl = pdf.add_table_cell(tbl, 3, 4, L"文件zip格式接受", optlist);
	tbl = pdf.add_table_cell(tbl, 3, 5, L"薄层能打开的分期（=1.25mm）数量为：1个", optlist);
	tbl = pdf.add_table_cell(tbl, 3, 6, L"图像采集完整", optlist);
	tbl = pdf.add_table_cell(tbl, 3, 7, L"JPG格式", optlist);

	optlist = L"stroke={{line=frame linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}} {line=other linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}}}";
	result = pdf.fit_table(tbl, 56.5, 126, 538.5, 504, optlist);
	pdf.get_p().delete_table(tbl, L"");

	optlist = L"fontname=simsun fontsize=10 encoding=unicode position={center}";
	pdf.get_p().fit_textline(L"6", a4_width / 2, 787, optlist);

	pdf.get_p().end_page_ext(L"");
}

void generate_page7()
{
	int tf = -1;
	int tbl = -1;
	wstring tf_text;
	wstring optlist;
	wstring result;

	pdf.get_p().begin_page_ext(a4_width, a4_height, L"topdown=true");

	// table 1
	optlist = L"fittextline={position={center top} fontname=msyhbd fontsize=12} colwidth=482 rowheight=42 margin=12";
	tbl = pdf.add_table_cell(tbl, 1, 1, L"3D 建模可视化说明", optlist);

	optlist = L"fontname=msyh fontsize=10.5 leftindent=18 parindent=0 alignment=justify leading=100%";
	tf_text = L"根据CT图像：\n"
		"在薄层扫描期重建了";
	tf = pdf.add_textflow(tf, tf_text, optlist);

	optlist = L"fontname=msyh fontsize=10.5 leftindent=18 parindent=0 alignment=justify leading=300% underline";
	tf_text = L"肺、肺动脉、肺静脉、支气管、占位、对肺进行肺叶、根据支气管对肺进行分段";
	tf = pdf.add_textflow(tf, tf_text, optlist);

	optlist = L"fontname=msyh fontsize=10.5 leftindent=18 parindent=0 alignment=justify leading=300% nounderline";
	tf_text = L"等模型。\n";
	tf = pdf.add_textflow(tf, tf_text, optlist);

	tf_text = L"备注：\n"
		"左肺上叶、左肺下叶是通过左肺斜裂进行划分；右肺上叶、右肺中叶是通过右肺水平裂进行划分，右肺中叶、右肺下叶是通过右肺斜裂进行划分。\n"
		"左肺上叶肺份段是通过支气管进行划分。\n";
	tf = pdf.add_textflow(tf, tf_text, optlist);

	tf_text = L"说明：\n"
		"1、CT数据层厚1. 25mm；参与重建有效CT片265张，合计使用CT数据265张。\n"
		"2、由于肿瘤边界变化，重建的病灶模型大小与实际手术中见到的病变大小可能存在一定的偏差，重建仅供参考，请结合临床。\n";
	tf = pdf.add_textflow(tf, tf_text, optlist);

	optlist = L"fontname=msyhbd fontsize=10.5 leftindent=0 parindent=18 alignment=justify leading=300%";
	tf_text = L"声明：此报告基于CT原始DICOM数据3D建模所得，3D重建结果严格依据CT/MRI原始图像数据，空间位置关系和量化特征数据仅供参考，不作为临床诊断依据和评价指标。";
	tf = pdf.add_textflow(tf, tf_text, optlist);

	optlist = L"textflow=" + int_to_wstr(tf) + L" fittextflow={verticalalign=top} colwidth=482 margin=8";
	tbl = pdf.add_table_cell(tbl, 1, 2, L"", optlist);

	optlist = L"stroke={{line=frame linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}} {line=other linewidth=0.3 strokecolor={rgb 0.741176 0.839215 0.933333}}}";
	result = pdf.fit_table(tbl, 56.5, 72, 538.5, 530, optlist);
	pdf.get_p().delete_table(tbl, L"");

	optlist = L"fontname=msyh fontsize=11 encoding=unicode alignment=center";
	tf = -1;
	tf = pdf.add_textflow(tf, L"3D重建工程师：XXX", optlist);
	tf = pdf.add_textflow(tf, L"\u3000\u3000\u3000\u3000\u3000\u3000", optlist);
	tf = pdf.add_textflow(tf, L"报告人：XXX", optlist);
	tf = pdf.add_textflow(tf, L"\u3000\u3000\u3000\u3000\u3000\u3000", optlist);
	tf = pdf.add_textflow(tf, L"审核人：XXX", optlist);
	result = pdf.get_p().fit_textflow(tf, 0, 560, a4_width, 580, L"verticalalign={center}");
	pdf.get_p().delete_textflow(tf);

	optlist = L"fontname=simsun fontsize=10 encoding=unicode position={center}";
	pdf.get_p().fit_textline(L"7", a4_width / 2, 787, optlist);

	pdf.get_p().end_page_ext(L"");
}

int main()
{
	try {

		init();

		//if (pdf.get_p().begin_document(outfile, L"compatibility=1.7ext3") == -1) {
		if (pdf.get_p().begin_document(outfile, L"") == -1) {
			throw CreateFileException(outfile, pdf.get_p().get_errmsg());
		}

		generate_page1();
		generate_page2();
		generate_page3();
		generate_page4();
		generate_page5();
		generate_page6();
		generate_page7();

		pdf.get_p().end_document(L"");
	}
	catch (PDFlib::Exception& ex) {
		wcerr << L"PDFlib exception occurred:" << endl
			<< L"[" << ex.get_errnum() << L"] " << ex.get_apiname()
			<< L": " << ex.get_errmsg() << endl;
		return -1;
	}
	catch (exception& e) {
		cerr << e.what() << endl;
		return -1;
	}
	catch (...) {
		cerr << "Generic C++ exception occurred!" << endl;
		return -1;
	}

	return 0;
}
