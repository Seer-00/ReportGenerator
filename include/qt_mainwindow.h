#pragma once

#include <QtWidgets/QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <QDesktopServices>
#include <QUrl>

#include "ui_MainWindow.h"
#include "function.h"

#define LOG(msg) (log(msg))
#define PROGRESS(percent) (ui.progressBar->setValue(percent))

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = Q_NULLPTR);
	~MainWindow();
	// 根据导入的配置文件，更新主界面的输入、输出信息
	void update_config_info(const QString& config_path);
	// 根据 config 拼接 field 相应的字符串
	string get_field_str(const char* field, int field_type);
	// 追加日志显示
	void log(const string& msg);

private:
	Ui::MainWindow ui;

private slots:
	void onAcLoadConfigTriggered();  // 菜单栏：载入配置文件  被点击
	void onPBShowModelsClicked();    // 按钮：显示3D模型      被点击
	void onPBGenerateViewsClicked(); // 按钮：仅生成视图      被点击
	void onPBGenerateDataClicked();  // 按钮：仅生成3D数据    被点击
	void onPBGeneratePDFClicked();   // 按钮：一键生成PDF报告 被点击

	void onPBEditConfigClicked();       // 按钮：编辑配置文件 被点击
	void onPBEditTemplateClicked();     // 按钮：编辑模板文件 被点击
	void onPBEditUserInputClicked();    // 按钮：编辑用户输入 被点击
	void onPBEditDefaultInputClicked(); // 按钮：编辑默认输入 被点击
	void onPBEditInputFlagsClicked();   // 按钮：编辑对应规则 被点击

	void onPBOpenDirClicked();       // 按钮：打开输出目录    被点击
	void onPBOpenPDFClicked();       // 按钮：打开PDF报告     被点击
};
