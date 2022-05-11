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
	// ���ݵ���������ļ�����������������롢�����Ϣ
	void update_config_info(const QString& config_path);
	// ���� config ƴ�� field ��Ӧ���ַ���
	string get_field_str(const char* field, int field_type);
	// ׷����־��ʾ
	void log(const string& msg);

private:
	Ui::MainWindow ui;

private slots:
	void onAcLoadConfigTriggered();  // �˵��������������ļ�  �����
	void onPBShowModelsClicked();    // ��ť����ʾ3Dģ��      �����
	void onPBGenerateViewsClicked(); // ��ť����������ͼ      �����
	void onPBGenerateDataClicked();  // ��ť��������3D����    �����
	void onPBGeneratePDFClicked();   // ��ť��һ������PDF���� �����

	void onPBEditConfigClicked();       // ��ť���༭�����ļ� �����
	void onPBEditTemplateClicked();     // ��ť���༭ģ���ļ� �����
	void onPBEditUserInputClicked();    // ��ť���༭�û����� �����
	void onPBEditDefaultInputClicked(); // ��ť���༭Ĭ������ �����
	void onPBEditInputFlagsClicked();   // ��ť���༭��Ӧ���� �����

	void onPBOpenDirClicked();       // ��ť�������Ŀ¼    �����
	void onPBOpenPDFClicked();       // ��ť����PDF����     �����
};
