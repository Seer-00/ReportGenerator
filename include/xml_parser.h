#pragma once
#ifndef XML_PARSER_H
#define XML_PARSER_H

#define DEFAULT  "__DEFAULT__"
#define GENERATE "__GENERATE__"

#define _NODE_BEG_PAGE      10
#define _NODE_END_PAGE      11
#define _NODE_IMAGE         100
#define _NODE_TABLE         101
#define _NODE_TEXTLINE      102
#define _NODE_TEXTFLOW      103
#define _NODE_ANNOTATION_3D 104

#include <map>

#include "pugixml/pugiconfig.hpp"
#include "pugixml/pugixml.hpp"

#include "program_exception.h"
#include "pdf_generator.h"
#include "json_parser.h"

using namespace std;
using namespace pugi;

// 生成的 PLY 文件的写路径
#define PLY_MODEL_PATH(outdir) (outdir + "/temp/enabled_models.ply")
// 生成的 U3D 文件的读路径
#define U3D_MODEL_PATH(outdir) (outdir + "/temp/enabled_models.u3d")
// PLY -> U3D 的 .bat 路径
#define PLY_TO_U3D_PATH (L".\\resource\\PLY_TO_U3D.bat")

struct Labels // 专用于生成 PDF 报告的 page2 的 "颜色标示" 部分
{
// 拼接“颜色标示”图片的读写路径 
#define LABEL_IMAGE_PATH(outdir, name) (outdir + "/temp/" + name + ".png")
// “颜色标示”中纯白色默认图片的名字
#define LABEL_IMAGE_WHITE "white_default"
#define LABEL_W 22.0 // 像素
#define LABEL_H 10.0 // 像素
    int idx = 1;                // 用于插入 name_map 时，作为第一项
    map<int, string> name_map;  // int 对应第几项，string 对应 STL 模型文件的名称
};

class XMLParser
{
public:
    XMLParser();
    ~XMLParser();

    void parse_file(const string& filename);

protected:
    xml_document xml;
};

class TemplateParser : public XMLParser // 解析 template_default.xml 文件，单例（一次运行中只能使用一份该文件）
{
public:
    static TemplateParser* get_instance();

    void init();

    void parse_template();

    void parse_node_page(xml_node& page_node);
    void parse_node_beg_page(xml_node& beg_page_node);
    void parse_node_end_page(xml_node& end_page_node);
    int  parse_node_img(xml_node& img_node);
    void parse_node_tbl(xml_node& tbl_node);
    void parse_node_tbl_cell(xml_node& tbl_cell_node, int& tbl);
    void parse_node_tl(xml_node& tl_node);
    int  parse_node_tf(xml_node& tf_node);
    void parse_node_tf_item(xml_node& tf_item_node, int& tf);
    int  parse_node_3d_view(xml_node& view_3d_node);
    int  parse_node_3d_data(xml_node& data_3d_node, vector<int>& views);
    void parse_node_3d_annotation(xml_node& annotation_3d_node);

    void handle_flag(xml_node& _node, string& field, const char* _childname);
    string handle_inputflag(xml_node& _node, const char* _childname);
    string handle_generateflag(xml_node& _node, const char* _childname);
    string handle_escapechar(xml_node& _node, string text);

private:
    TemplateParser();
    ~TemplateParser();

    bool is_enabled(vector<string>& pages, const string& id);

private:
    map<string, int> switch_map;
};

class ModelInfoParser : public XMLParser // 解析 model_info.xml 文件，单例（一次运行中只能使用一份该文件）
{
public:
    static ModelInfoParser* get_instance();

    xml_node get_model_node(const string& name, const string& type);

    string get_organ_name(xml_node& model_node);
    double get_opacity(xml_node& model_node);
    bool get_visible(xml_node& model_node);
    void get_color(xml_node& model_node, double* color, int size);

private:
    ModelInfoParser();
    ~ModelInfoParser();
};

#endif // !XML_PARSER_H


