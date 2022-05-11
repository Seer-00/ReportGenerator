#include "qt_mainwindow.h"

extern JSONParser config;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("PDF Report Generator");

    // 连接信号与槽
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

    // 初始化：自动导入配置文件
    try {
        load_config(DEFAULT_CONFIG_PATH);
        update_config_info(s2q(DEFAULT_CONFIG_PATH));
        LOG("加载默认配置文件成功 " + string(DEFAULT_CONFIG_PATH));
    }
    catch (exception& e) {
        QMessageBox::warning(this, s2q("警告"), s2q("未能加载默认的配置文件"));
        LOG(e.what());
        LOG("加载默认配置文件失败");
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
    if (fields.size() == 0) return "无";

    string ret = "";
    if (field_type == 1) { // Array
        for (unsigned int i = 0; i < fields.size(); i++) {
            ret.append(fields[i].asString()).append(", ");
        }
    }
    if (field_type == 2) { // Json
        for (string name : fields.getMemberNames()) {
            ret.append(utf8_to_str(name)).append("，");
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
    // 使Qt正确显示中文：s2q (i.e. string to qstring)
    QString file = QFileDialog::getOpenFileName(this, s2q("选择配置文件"), "./", "Config (*.json)");
    if (file.isEmpty()) { return; }

    try {
        load_config(q2s(file));
        update_config_info(file);
        LOG("加载配置文件成功 " + q2s(file));
    }
    catch (exception& e) {
        QMessageBox::warning(this, s2q("警告"), s2q("加载配置文件失败"));
        LOG(e.what());
        LOG("加载配置文件失败 " + q2s(file));
    }
}

void MainWindow::onPBShowModelsClicked()
{
    LOG("用户操作：显示3D模型");
    PROGRESS(0);
    PROGRESS(100);
    show_models();
}

void MainWindow::onPBGenerateViewsClicked()
{
    LOG("用户操作：仅生成视图");
    PROGRESS(0);
    generate_views();
    PROGRESS(100);
    LOG("已成功生成视图文件");

    if (ui.acAutoShowViewDir->isChecked()) {
        onPBOpenDirClicked(); // 相当于点击了一次打开输出目录
    }
}

void MainWindow::onPBGenerateDataClicked()
{
    LOG("用户操作：仅生成3D数据");
    PROGRESS(0);
    // 生成 .ply
    generate_PLY();
    LOG("已成功生成PLY文件");
    PROGRESS(50);
    // 根据用户选项，判断是否调用 .bat 脚本，进行 .ply -> .u3d
    if (ui.acAutoU3D->isChecked()) {
        LOG("调用脚本 " + wstr2str(PLY_TO_U3D_PATH) + " 自动转换 PLY -> U3D ...");
        convert_PLY_TO_U3D();
        LOG("已成功生成U3D文件");
    }
    PROGRESS(100);

    if (ui.acAutoShowDataDir->isChecked()) {
        onPBOpenDirClicked(); // 相当于点击了一次打开输出目录
    }
}

void MainWindow::onPBGeneratePDFClicked()
{
    LOG("用户操作：一键生成PDF报告");
    PROGRESS(0);
    try {
        // 生成视图
        generate_views();
        LOG("已成功生成视图文件");
        PROGRESS(25);
        // 生成模型文件
        generate_PLY();
        LOG("已成功生成PLY文件");
        PROGRESS(50);
        // 获得必需的 .u3d 文件
        if (ui.acAutoU3D->isChecked()) {
            LOG("调用脚本 " + wstr2str(PLY_TO_U3D_PATH) + " 执行 PLY -> U3D 的转换");
            convert_PLY_TO_U3D();
            LOG("已成功生成U3D文件");
        }
        else {
            string out  = q2s(ui.leOutputDir->text());
            string path = U3D_MODEL_PATH(out);
            QMessageBox::information(this, s2q("提示"), s2q("请手动转换U3D文件，存放为：\n" + path));
        }
        PROGRESS(75);
        // 生成 PDF 报告
        generate_report();
        PROGRESS(100);

        string out = q2s(ui.leOutputDir->text()) + "/" + q2s(ui.leReportName->text());
        LOG("已成功生成PDF报告：" + out);

        if (ui.acAutoShowPDF->isChecked()) {
            onPBOpenPDFClicked(); // 相当于点击了一次打开 PDF 报告
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
        LOG("出现未知异常，程序中断 ...");
    }
}

void MainWindow::onPBEditConfigClicked()
{
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(ui.leConfig->text()));
    if (!ret) {
        QMessageBox::warning(this, s2q("警告"), s2q("未能打开合适的软件，以编辑配置文件"));
    }
}

void MainWindow::onPBEditTemplateClicked()
{
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(ui.leTemplate->text()));
    if (!ret) {
        QMessageBox::warning(this, s2q("警告"), s2q("未能打开合适的软件，以编辑模板文件"));
    }
}

void MainWindow::onPBEditUserInputClicked()
{
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(ui.leUserInput->text()));
    if (!ret) {
        QMessageBox::warning(this, s2q("警告"), s2q("未能打开合适的软件，以编辑用户输入"));
    }
}

void MainWindow::onPBEditDefaultInputClicked()
{
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(ui.leDefaultInput->text()));
    if (!ret) {
        QMessageBox::warning(this, s2q("警告"), s2q("未能打开合适的软件，以编辑默认输入"));
    }
}

void MainWindow::onPBEditInputFlagsClicked()
{
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(ui.leInputFlags->text()));
    if (!ret) {
        QMessageBox::warning(this, s2q("警告"), s2q("未能打开合适的软件，以编辑对应规则"));
    }
}

void MainWindow::onPBOpenDirClicked()
{
    QString path = ui.leOutputDir->text();
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(path));
    if (!ret) {
        QMessageBox::warning(this, s2q("警告"), s2q("未能打开输出目录"));
    }
}

void MainWindow::onPBOpenPDFClicked()
{
    QString file = ui.leOutputDir->text() + "/" + ui.leReportName->text();
    bool ret = QDesktopServices::openUrl(QUrl::fromLocalFile(file));
    if (!ret) {
        QMessageBox::warning(this, s2q("警告"), s2q("未能打开合适的软件，以查看PDF报告"));
    }
}
