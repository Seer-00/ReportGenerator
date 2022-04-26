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

class XMLParser // 单例，用于解析文件：模板.xml 
{
public:
    static XMLParser* get_instance();
    void parse_xml_document();

    void parse_node_page(xml_node page_node);
    void parse_node_beg_page(xml_node beg_page_node);
    void parse_node_end_page(xml_node end_page_node);
    int  parse_node_img(xml_node img_node);
    void parse_node_tbl(xml_node tbl_node);
    void parse_node_tbl_cell(xml_node tbl_cell_node, int& tbl);
    void parse_node_tl(xml_node tl_node);
    int  parse_node_tf(xml_node tf_node);
    void parse_node_tf_item(xml_node tf_item_node, int& tf);
    int  parse_node_3d_view(xml_node view_3d_node);
    int  parse_node_3d_data(xml_node data_3d_node, vector<int>& views);
    void parse_node_3d_annotation(xml_node annotation_3d_node);

    string handle_inputflag(xml_node _node, const char* _childname);

    string handle_escapechar(xml_node _node, string text);

private:
    XMLParser();
    ~XMLParser();

    bool is_enabled(vector<string>& pages, const string& id);

private:
    xml_document xml;
    map<string, int> switch_map;
};

#endif // !XML_PARSER_H


