#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H

#include "pdf_generator.h"
#include "xml_parser.h"
#include "json_parser.h"
#include "stl_parser.h"

void init();

void generate_report();

void show_models();

void generate_views();

void generate_PLY();

void convert_PLY_TO_U3D();

#endif // !FUNCTION_H

