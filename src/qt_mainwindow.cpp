#include "qt_mainwindow.h"

extern JSONParser config;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("PDF Report Generator");

    // �����ź����
    connect(ui.acLoadConfig, &QAction::triggered, this, &MainWindow::onAcLoadConfigTriggered);
    connect(ui.pbShowModels, &QPushButton::clicked, this, &MainWindow::onPBShowModelsClicked);
    connect(ui.pbGenerateViews, &QPushButton::clicked, this, &MainWindow::onPBGenerateViewsClicked);
    connect(ui.pbGenerateData, &QPushButton::clicked, this, &MainWindow::onPBGenerateDataClicked);
    connect(ui.pbGeneratePDF, &QPushButton::clicked, this, &MainWindow::onPBGeneratePDFClicked);
    connect(ui.pbEditConfig, &QPushButton::clicked, this, &MainWindow::onPBEditConfigClicked);
    connect(ui.pbEditTemplate, &QPushButton::clicked, this, &MainWindow::onPBEditTemplateClicked);
    connect(ui.pbEditUserInput, &QPushButton::clicked, this, &MainWindow::onPBEditUserInputClicked);
    connect(ui.pbEditDefaultInput, &QPushButton::clicked, this, &MainWindow::onPBEditDefaultInputClicked);
    connect(ui.pbEditInputFlags, &QPushButton::clicked, this, &MainWindow::onPBEditInputFlagsClicked);
    connect(ui.pbOpenDir, &QPushButton::clicked, this, &MainWindow::onPBOpenDirClicked);
    connect(ui.pbOpenPDF, &QPushButton::clicked, this, &MainWindow::onPBOpenPDFClicked);

    // ��ʼ�����Զ����������ļ�
    try {
        load_config(DEFAULT_CONFIG_PATH);
        update_config_info(s2q(DEFAULT_CONFIG_PATH));
        LOG("����Ĭ�������ļ��ɹ� " + string(DEFAULT_CONFIG_PATH));
    }
    catch (exception& e) {
        QMessageBox::warning(this, s2q("����"), s2q("δ�ܼ���Ĭ�ϵ������ļ�"));
        LOG(e.what());
        LOG("����Ĭ�������ļ�ʧ��");
    }
}

MainWindow::~MainWindow() { }

void MainWindow::update_config_info(const QString& config_path)
{
    ui.leConfig->setText(config_path);

	Value cfg = config.get_root();
    const string res_report_template = utf8_to_str(cfg["res_report_template"].asString());
	const string res_input_user      = utf8_to_str(cfg["res_input_user"].asString());
	const string res_input_dft       = utf8_to_str(cfg["res_input_dft"].asString());
	const string res_inputflags      = utf8_to_str(cfg["res_inputflags"].asString());
    const string enabled_pages       = get_field_str("enabled_pages", 1);
    const string enabled_models      = get_field_str("enabled_models", 2);
    const string enabled_views       = get_field_str("enabled_views", 2);
    const string output_dir          = utf8_to_str(cfg["output_dir"].asString());
    const string report_name         = utf8_to_str(cfg["report_name"].asString());

    ui.leTemplate->setText(s2q(res_report_template));
    ui.leUserInput->setText(s2q(res_input_user));
    ui.leDefaultInput->setText(s2q(res_input_dft));
    ui.leInputFlags->setText(s2q(res_inputflags));
    ui.leEnabledPages->setText(s2q(enabled_pages));
    ui.leEnabledModels->setText(s2q(enabled_models));
    ui.leEnabledViews->setText(s2q(enabled_views));
    ui.leOutputDir->setText(s2q(output_dir));
    ui.leReportName->setText(s2q(report_name));
}

string MainWindow::get_field_str(const char* field, int field_type)
{
    Value fields = config.get_root()[field];
    if (fields.size() == 0) return "��";

    string ret = "";
    if (field_type == 1) { // Array
        for (unsigned int i = 0; i < fields.size(); i++) {
            ret.append(fields[i].asString()).append(", ");
        }
    }
    if (field_type == 2) { // Json
        for (string name : fields.getMemberNames()) {
            ret.append(utf8_to_str(name)).append("��");
        }
    }
    return ret.substr(0, ret.length() - 2); // e.g. "a, b, c, d"
}

void MainWindow::log(const string& msg)
{
    QTime current_time = QTime::currentTime();
    QString time = current_time.toString("hh:mm:ss");
    ui.tbLog->append(time + s2q(" " + msg));
}

void MainWindow::onAcLoadConfigTriggered()
{
    // ʹQt��ȷ��ʾ���ģ�s2q (i.e. string to qstring)
    QString file = QFileDialog::getOpenFileName(this, s2q("ѡ�������ļ�"), "./", "Config (*.json)");
    if (file.isEmpty()) { return; }

    try {
        load_config(q2s(file));
        update_config_info(file);
        LOG("���������ļ��ɹ� " + q2s(file));
    }
    catch (exception& e) {
        QMessageBox::warning(this, s2q("����"), s2q("���������ļ�ʧ��"));
        LOG(e.what());
        LOG("���������ļ�ʧ�� " + q2s(file));
    }
}

void MainWindow::onPBShowModelsClicked()
{
    LOG("�û���������ʾ3Dģ��");
    PROGRESS(0);
    PROGRESS(100);
    show_models();
}

void MainWindow::onPBGenerateViewsClicked()
{
    LOG("�û���������������ͼ");
    PROGRESS(0);
    generate_views();
    PROGRESS(100);
    LOG("�ѳɹ�������ͼ�ļ�");

    if (ui.acAutoShowViewDir->isChecked()) {
        onPBOpenDirClicked(); // �൱�ڵ����һ�δ����Ŀ¼
    }
}

void MainWindow::onPBGenerateDataClicked()
{
    LOG("�û�������������3D����");
    PROGRESS(0);
    // ���� .ply
    generate_PLY();
    LOG("�ѳɹ�����PLY�ļ�");
    PROGRESS(50);
    // �����û�ѡ��ж��Ƿ���� .bat �ű������� .ply -> .u3d
    if (ui.acAutoU3D->isChecked()) {
        LOG("���ýű� " + wstr2str(PLY_TO_U3D_PATH) + " �Զ�ת�� PLY -> U3D ...");
        convert_PLY_TO_U3D();
        LOG("�ѳɹ�����U3D�ļ�");
    }
    PROGRESS(100);

    if (ui.acAutoShowDataDir->isChecked()) {
        onPBOpenDirClicked(); // �൱�ڵ����һ�δ����Ŀ¼
    }
}

void MainWindow::onPBGeneratePDFClicked()
{
    LOG("�û�������һ������PDF����");
    PROGRESS(0);
    try {
        // ������ͼ
        generate_views();
        LOG("�ѳɹ�������ͼ�ļ�");
        PROGRESS(25);
        // ����ģ���ļ�
        generate_PLY();
        LOG("�ѳɹ�����PLY�ļ�");
        PROGRESS(50);
        // ��ñ���� .u3d �ļ�
        if (ui.acAutoU3D->isChecked()) {
            LOG("���ýű� " + wstr2str(PLY_TO_U3D_PATH) + " ִ�� PLY -> U3D ��ת��");
            convert_PLY_TO_U3D();
            LOG("�ѳɹ�����U3D�ļ�");
        }
        else {
            string out  = q2s(ui.leOutputDir->text());
            string path = U3D_MODEL_PATH(out);
            QMessageBox::information(this, s2q("��ʾ"), s2q("���ֶ�ת��U3D�ļ������Ϊ��\n" + path));
        }
        PROGRESS(75);
        // ���� PDF ����
        generate_report();
        PROGRESS(100);

        string out = q2s(ui.leOutputDir->text()) + "/" + q2s(ui.leReportName->text());
        LOG("�ѳɹ�����PDF���棺" + out);

        if (ui.acAutoShowPDF->isChecked()) {
            onPBOpenPDFClicked(); // �൱�ڵ����һ�δ� PDF ����
        }
    }
    catch (PDFlib::Exception& ex) {
        PROGRESS(0);
        string msg;
        msg.append("PDFlib exception occurred [").append(int2str(ex.get_errnum())).append("] ")
            .append(wstr2str(ex.get_apiname())).append(": ").append(wstr2str(ex.get_errmsg()));
        LOG(msg);
    }
    catch (exception& e) {
        PROGRESS(0);
        LOG(e.what());
    }
    catch (...) {
        PROGRESS(0);
        LOG("����δ֪�쳣�������ж� ...");
    }
}

void MainWindow::onPBEditConfigClicked()
{
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(ui.leConfig->text()));
    if (!ret) {
        QMessageBox::warning(this, s2q("����"), s2q("δ�ܴ򿪺��ʵ�������Ա༭�����ļ�"));
    }
}

void MainWindow::onPBEditTemplateClicked()
{
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(ui.leTemplate->text()));
    if (!ret) {
        QMessageBox::warning(this, s2q("����"), s2q("δ�ܴ򿪺��ʵ�������Ա༭ģ���ļ�"));
    }
}

void MainWindow::onPBEditUserInputClicked()
{
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(ui.leUserInput->text()));
    if (!ret) {
        QMessageBox::warning(this, s2q("����"), s2q("δ�ܴ򿪺��ʵ�������Ա༭�û�����"));
    }
}

void MainWindow::onPBEditDefaultInputClicked()
{
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(ui.leDefaultInput->text()));
    if (!ret) {
        QMessageBox::warning(this, s2q("����"), s2q("δ�ܴ򿪺��ʵ�������Ա༭Ĭ������"));
    }
}

void MainWindow::onPBEditInputFlagsClicked()
{
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(ui.leInputFlags->text()));
    if (!ret) {
        QMessageBox::warning(this, s2q("����"), s2q("δ�ܴ򿪺��ʵ�������Ա༭��Ӧ����"));
    }
}

void MainWindow::onPBOpenDirClicked()
{
    QString path = ui.leOutputDir->text();
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    if (!ret) {
        QMessageBox::warning(this, s2q("����"), s2q("δ�ܴ����Ŀ¼"));
    }
}

void MainWindow::onPBOpenPDFClicked()
{
    QString file = ui.leOutputDir->text() + "/" + ui.leReportName->text();
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(file));
    if (!ret) {
        QMessageBox::warning(this, s2q("����"), s2q("δ�ܴ򿪺��ʵ�������Բ鿴PDF����"));
    }
}
