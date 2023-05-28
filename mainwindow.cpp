#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->visualizer->layout()->addWidget(&visualizer); //add cloud visualizer to ui
    visualizer.setGeometry(1, 1, visualizer.width(), visualizer.height());
    visualizer.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    visualizer.setFocusPolicy(Qt::StrongFocus);
    visualizer.addClouds(&cloudsXYZ, &cloudsXYZI, &cloudsXYZRGB);
    visualizer.update();

    ui->listClouds->setEditTriggers(QAbstractItemView::NoEditTriggers); //shortcut to sellect all clouds in list
    selectAll = new QShortcut(QKeySequence(tr("Ctrl+A")), this);

    connect (selectAll, &QShortcut::activated, [this]()
    {
       for(auto &cloud : cloudsXYZ.clouds)
       {
           cloud.second = true;
       }
       for(auto &cloud : cloudsXYZI.clouds)
       {
           cloud.second = true;
       }
       for(auto &cloud : cloudsXYZRGB.clouds)
       {
           cloud.second = true;
       }
       MainWindow::updateClouds();
    });

    deleteS = new QShortcut(QKeySequence(tr("Ctrl+Backspace")), this); //shortcut to delete selected clouds

    connect (deleteS, &QShortcut::activated, [this]()
    {
        for(int i = 0; i < cloudsXYZ.clouds.size(); i++)
        {
            if(cloudsXYZ.clouds.at(i).second)
            {
                cloudsXYZ.names.erase(cloudsXYZ.names.begin() + i);
                cloudsXYZ.clouds.erase(cloudsXYZ.clouds.begin() + i);
                i--;
            }
        }
        for(int i = 0; i < cloudsXYZI.clouds.size(); i++)
        {
            if(cloudsXYZI.clouds.at(i).second)
            {
                cloudsXYZI.names.erase(cloudsXYZI.names.begin() + i);
                cloudsXYZI.clouds.erase(cloudsXYZI.clouds.begin() + i);
                i--;
            }
        }
        for(int i = 0; i < cloudsXYZRGB.clouds.size(); i++)
        {
            if(cloudsXYZRGB.clouds.at(i).second)
            {
                cloudsXYZRGB.names.erase(cloudsXYZRGB.names.begin() + i);
                cloudsXYZRGB.clouds.erase(cloudsXYZRGB.clouds.begin() + i);
                i--;
            }
        }
        MainWindow::updateClouds();
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changedStatus(QCheckBox *checkBox) //selecting clouds
{
    QString name = "";
    QString type = "";
    QString obj_name = checkBox->objectName();
    int i = 0;
    while(obj_name[i] != '_')
    {
        i++;
    }
    i++;
    while(obj_name[i] != '_')
    {
        type += obj_name[i];
        i++;
    }
    i++;
    for(i; i < obj_name.size(); i++)
    {
        name += obj_name[i];
    }
    if(type == "xyz")
    {
        cloudsXYZ.clouds.at(std::distance(cloudsXYZ.names.begin(), std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), name))).second = checkBox->isChecked();
    }
    else if(type == "xyzi")
    {
        cloudsXYZI.clouds.at(std::distance(cloudsXYZI.names.begin(), std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), name))).second = checkBox->isChecked();
    }
    else if(type == "xyzrgb")
    {
        cloudsXYZRGB.clouds.at(std::distance(cloudsXYZRGB.names.begin(), std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), name))).second = checkBox->isChecked();
    }
    visualizer.update();
}

void MainWindow::deleteCloud(QPushButton *pushButton)
{
    QString name = "";
    QString type = "";
    QString obj_name = pushButton->objectName();
    int i = 0;
    while(obj_name[i] != '_')
    {
        i++;
    }
    i++;
    while(obj_name[i] != '_')
    {
        type += obj_name[i];
        i++;
    }
    i++;
    for(i; i < obj_name.size(); i++)
    {
        name += obj_name[i];
    }
    if(type == "xyz")
    {
        int del = std::distance(cloudsXYZ.names.begin(), std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), name));
        cloudsXYZ.clouds.erase(cloudsXYZ.clouds.begin() + del);
        cloudsXYZ.names.erase(cloudsXYZ.names.begin() + del);
        visualizer.deleteXYZ(del);
    }
    else if(type == "xyzi")
    {
        int del = std::distance(cloudsXYZI.names.begin(), std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), name));
        cloudsXYZI.clouds.erase(cloudsXYZI.clouds.begin() + del);
        cloudsXYZI.names.erase(cloudsXYZI.names.begin() + del);
        visualizer.deleteXYZI(del);
    }
    else if(type == "xyzrgb")
    {
        int del = std::distance(cloudsXYZRGB.names.begin(), std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), name));
        cloudsXYZRGB.clouds.erase(cloudsXYZRGB.clouds.begin() + del);
        cloudsXYZRGB.names.erase(cloudsXYZRGB.names.begin() + del);
        visualizer.deleteXYZRGB(del);
    }
    MainWindow::updateClouds();
    visualizer.update();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
}

void MainWindow::updateClouds()
{
    ui->listClouds->setMaximumSize(280, ui->listClouds->maximumHeight());
    ui->listClouds->setMinimumSize(280, ui->listClouds->minimumHeight());
    ui->listClouds->resize(280, ui->listClouds->height());
    bool empty = (cloudsXYZ.clouds.empty() && cloudsXYZI.clouds.empty() && cloudsXYZRGB.clouds.empty());
    QTableWidget *table = ui->listClouds;
    if(!empty)
    {
        table->setColumnCount(4);
        table->setColumnWidth(0, 20);
        table->setColumnWidth(1, 0.55 * table->width());
        table->setColumnWidth(2, 0.25 * table->width());
        table->setColumnWidth(3, 20);
        QStringList header;
        header << "+" << "Name" << "Type" << "-";
        table->setHorizontalHeaderLabels(header);

        int colOfClouds = 1;

        for(int i = 0; i < cloudsXYZ.clouds.size(); i++)
        {
            table->setRowCount(colOfClouds);

            QCheckBox *check = new QCheckBox();
            QTableWidgetItem *name = new QTableWidgetItem(cloudsXYZ.names.at(i));
            QTableWidgetItem *type = new QTableWidgetItem(tr("XYZ"));
            QPushButton *del = new QPushButton(tr("-"));
            del->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
            check->setCheckState((cloudsXYZ.clouds.at(i).second? Qt::Checked : Qt::Unchecked));

            QString widgetSelectName = "opened_xyz_" + cloudsXYZ.names[i];
            QString widgetDeleteName = "delete_xyz_" + cloudsXYZ.names[i];

            check->setObjectName(widgetSelectName);
            del->setObjectName(widgetDeleteName);

            table->setCellWidget(colOfClouds - 1, 0, check);
            table->setItem(colOfClouds - 1, 1, name);
            table->setItem(colOfClouds - 1, 2, type);
            table->setCellWidget(colOfClouds - 1, 3, del);

            connect(check, &QCheckBox::toggled, [this, check](){
                MainWindow::changedStatus(check);
            });

            connect(del, &QPushButton::clicked, [this, del](){
                MainWindow::deleteCloud(del);
            });

            colOfClouds++;
        }

        for(int i = 0; i < cloudsXYZI.clouds.size(); i++)
        {
            table->setRowCount(colOfClouds);

            QCheckBox *check = new QCheckBox();
            QTableWidgetItem *name = new QTableWidgetItem(cloudsXYZI.names.at(i));
            QTableWidgetItem *type = new QTableWidgetItem(tr("I"));
            QPushButton *del = new QPushButton(tr("-"));
            del->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
            check->setCheckState((cloudsXYZI.clouds.at(i).second? Qt::Checked : Qt::Unchecked));

            QString widgetSelectName = "opened_xyzi_" + cloudsXYZI.names[i];
            QString widgetDeleteName = "delete_xyzi_" + cloudsXYZI.names[i];

            check->setObjectName(widgetSelectName);
            del->setObjectName(widgetDeleteName);

            table->setCellWidget(colOfClouds - 1, 0, check);
            table->setItem(colOfClouds - 1, 1, name);
            table->setItem(colOfClouds - 1, 2, type);
            table->setCellWidget(colOfClouds - 1, 3, del);

            connect(check, &QCheckBox::toggled, [this, check](){
                MainWindow::changedStatus(check);
            });

            connect(del, &QPushButton::clicked, [this, del](){
                MainWindow::deleteCloud(del);
            });

            colOfClouds++;
        }
        for(int i = 0; i < cloudsXYZRGB.clouds.size(); i++)
        {
            table->setRowCount(colOfClouds);

            QCheckBox *check = new QCheckBox();
            QTableWidgetItem *name = new QTableWidgetItem(cloudsXYZRGB.names.at(i));
            QTableWidgetItem *type = new QTableWidgetItem(tr("RGB"));
            QPushButton *del = new QPushButton(tr("-"));
            del->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
            check->setCheckState((cloudsXYZRGB.clouds.at(i).second? Qt::Checked : Qt::Unchecked));

            QString widgetSelectName = "opened_xyzrgb_" + cloudsXYZRGB.names[i];
            QString widgetDeleteName = "delete_xyzrgb_" + cloudsXYZRGB.names[i];

            check->setObjectName(widgetSelectName);
            del->setObjectName(widgetDeleteName);

            table->setCellWidget(colOfClouds - 1, 0, check);
            table->setItem(colOfClouds - 1, 1, name);
            table->setItem(colOfClouds - 1, 2, type);
            table->setCellWidget(colOfClouds - 1, 3, del);

            connect(check, &QCheckBox::toggled, [this, check](){
                MainWindow::changedStatus(check);
            });

            connect(del, &QPushButton::clicked, [this, del](){
                MainWindow::deleteCloud(del);
            });

            colOfClouds++;
        }
    }
    else
    {
        table->clear();
        table->setColumnCount(0);
        table->setRowCount(0);
    }
}

void MainWindow::on_actionOpenCloud_triggered()
{
    QFileDialog dialog(this);
    dialog.setDirectory("/Users/larong/Desktop");
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("PLY files (*.ply)"));
    QStringList fileNames;
    if (dialog.exec())
    {
        fileNames = dialog.selectedFiles();
//        std::cout << fileNames.size() << " file(s) openes: " << std::endl;
        for(auto &name : fileNames)
        {
            std::fstream fileToRead;
            fileToRead.open(name.toStdString());

            QString fileName;
            for(char &c : name.toStdString())
            {
                if(c == '/')
                {
                    fileName = "";
                }
                else if(c == '.')
                {
                    break;
                }
                else
                {
                    fileName.push_back(c);
                }
            }

            std::string content;
            long long size;

            int type = 0;

            while(fileToRead>>content && content != "end_header")
            {
                if(content == "vertex")
                {
                    fileToRead>>content;
                    size = std::stoll(content);
                }
                if(content == "scalar_intensity")
                {
                    type = 1;
                }
                if(content == "red")
                {
                    type = 2;
                }
            }

            if(size != 0)
            {
                QString firstName = fileName;
                if(type == 0)
                {
                    pointCloud<point> openedCloud;
                    for(long long i = 0; i < size; i++)
                    {
                        float x, y, z;
                        fileToRead >> x >> y >> z;
                        openedCloud.push_back(point(x, y, z));
                    }
                    cloudsXYZ.clouds.push_back({openedCloud, 1});
                    int numberOfCopy = 0;
                    while(std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), fileName) != cloudsXYZ.names.end() ||
                          std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), fileName) != cloudsXYZI.names.end() ||
                          std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), fileName) != cloudsXYZRGB.names.end())
                    {
                        while(fileName != firstName)
                        {
                            fileName.resize(fileName.size() - 1);
                        }
                        fileName.push_back('_');
                        numberOfCopy++;
                        for(char c : std::to_string(numberOfCopy))
                        {
                            fileName.push_back(c);
                        }
                    }
//                    std::cout << fileName.toStdString() << std::endl;
                    cloudsXYZ.names.push_back(fileName);
                    visualizer.addCloudXYZ(ui->schemeXYZ->currentIndex());
                }
                else if(type == 1)
                {
                    pointCloud<pointI> openedCloud;
                    for(long long i = 0; i < size; i++)
                    {
                        float x, y, z, intensity;
                        fileToRead >> x >> y >> z >> intensity;
                        openedCloud.push_back(pointI(x, y, z, intensity));
                    }

                    cloudsXYZI.clouds.push_back({openedCloud, 1});
                    int numberOfCopy = 0;
                    while(std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), fileName) != cloudsXYZ.names.end() ||
                          std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), fileName) != cloudsXYZI.names.end() ||
                          std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), fileName) != cloudsXYZRGB.names.end())
                    {
                        while(fileName != firstName)
                        {
                            fileName.resize(fileName.size() - 1);
                        }
                        fileName.push_back('_');
                        numberOfCopy++;
                        for(char c : std::to_string(numberOfCopy))
                        {
                            fileName.push_back(c);
                        }
                    }
                    cloudsXYZI.names.push_back(fileName);
                    visualizer.addCloudXYZI(ui->schemeXYZI->currentIndex());
                }
                else if(type == 2)
                {
                    pointCloud<pointRGB> openedCloud;

                    for(long long i = 0; i < size; i++){
                        float x, y, z;
                        unsigned short r, g, b;
                        fileToRead >> x >> y >> z >> r >> g >> b;
                        openedCloud.push_back(pointRGB(x, y, z, r, g, b));
                    }

                    cloudsXYZRGB.clouds.push_back({openedCloud, 1});
                    int numberOfCopy = 0;
                    while(std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), fileName) != cloudsXYZ.names.end() ||
                          std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), fileName) != cloudsXYZI.names.end() ||
                          std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), fileName) != cloudsXYZRGB.names.end())
                    {
                        while(fileName != firstName)
                        {
                            fileName.resize(fileName.size() - 1);
                        }
                        fileName.push_back('_');
                        numberOfCopy++;
                        for(char c : std::to_string(numberOfCopy))
                        {
                            fileName.push_back(c);
                        }
                    }
                    cloudsXYZRGB.names.push_back(fileName);
                    visualizer.addCloudXYZRGB(ui->schemeRGB->currentIndex());
                }

            }
        }
        MainWindow::updateClouds();
    }
}

void MainWindow::on_actionSaveClouds_triggered()
{
    for(int i = 0; i < cloudsXYZ.names.size(); i++)
    {
        if(cloudsXYZ.clouds.at(i).second)
        {
            QFileDialog dialog;
            dialog.setFileMode(QFileDialog::AnyFile);
            QString dir = "/Users/larong/Desktop/" + cloudsXYZ.names.at(i);
            QString strFile = dialog.getSaveFileName(this, tr("Save point cloud"), dir , tr("PLY files (*.ply)"));
            QFile file(strFile);

            if(file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);

                out << "ply" << '\n' << "format ascii 1.0" << '\n';
                out << "comment Created by Cloud Visualizer " << VERSION << " (Larong)" << '\n';

                QDateTime date = QDateTime::currentDateTime();
                QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
                QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
                out << "comment Created " << formattedTime << '\n';

                out << "obj_info Generated by Cloud Visualizer!" << '\n';

                out << "element vertex " << cloudsXYZ.clouds.at(i).first.size() << '\n';

                out << "property float x" << '\n';
                out << "property float y" << '\n';
                out << "property float z" << '\n';

                out << "end_header" << '\n';

                for(unsigned long long j = 0; j < cloudsXYZ.clouds.at(i).first.size(); j++)
                {
                    out << cloudsXYZ.clouds.at(i).first[j].x() << ' ' << cloudsXYZ.clouds.at(i).first[j].y() << ' ' << cloudsXYZ.clouds.at(i).first[j].z() << '\n';
                }
            }

            file.close();
        }
    }
    for(int i = 0; i < cloudsXYZI.names.size(); i++)
    {
        if(cloudsXYZI.clouds.at(i).second)
        {
            QFileDialog dialog;
            dialog.setFileMode(QFileDialog::AnyFile);
            QString dir = "/Users/larong/Desktop/" + cloudsXYZI.names.at(i);
            QString strFile = dialog.getSaveFileName(this, tr("Save point cloud"), dir , tr("PLY files (*.ply)"));
            QFile file(strFile);

            if(file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);

                out << "ply" << '\n' << "format ascii 1.0" << '\n';
                out << "comment Created by Cloud Visualizer " << VERSION << " (Larong)" << '\n';

                QDateTime date = QDateTime::currentDateTime();
                QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
                QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
                out << "comment Created " << formattedTime << '\n';

                out << "obj_info Generated by Cloud Visualizer!" << '\n';

                out << "element vertex " << cloudsXYZI.clouds.at(i).first.size() << '\n';

                out << "property float x" << '\n';
                out << "property float y" << '\n';
                out << "property float z" << '\n';
                out << "property float scalar_intensity" << '\n';

                out << "end_header" << '\n';

                for(unsigned long long j = 0; j < cloudsXYZI.clouds.at(i).first.size(); j++)
                {
                    out << cloudsXYZI.clouds.at(i).first[j].x() << ' ' << cloudsXYZI.clouds.at(i).first[j].y() << ' ' << cloudsXYZI.clouds.at(i).first[j].z() << ' ' << cloudsXYZI.clouds.at(i).first[j].intensity() << '\n';
                }
            }

            file.close();
        }
    }
    for(int i = 0; i < cloudsXYZRGB.names.size(); i++)
    {
        if(cloudsXYZRGB.clouds.at(i).second)
        {
            QFileDialog dialog;
            dialog.setFileMode(QFileDialog::AnyFile);
            QString dir = "/Users/larong/Desktop/" + cloudsXYZRGB.names.at(i);
            QString strFile = dialog.getSaveFileName(this, tr("Save point cloud"), dir , tr("PLY files (*.ply)"));
            QFile file(strFile);

            if(file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QTextStream out(&file);

                out << "ply" << '\n' << "format ascii 1.0" << '\n';
                out << "comment Created by Cloud Visualizer " << VERSION << " (Larong)" << '\n';

                QDateTime date = QDateTime::currentDateTime();
                QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
                QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
                out << "comment Created " << formattedTime << '\n';

                out << "obj_info Generated by Cloud Visualizer!" << '\n';

                out << "element vertex " << cloudsXYZRGB.clouds.at(i).first.size() << '\n';

                out << "property float x" << '\n';
                out << "property float y" << '\n';
                out << "property float z" << '\n';
                out << "property uchar red" << '\n';
                out << "property uchar green" << '\n';
                out << "property uchar blue" << '\n';

                out << "end_header" << '\n';

                for(unsigned long long j = 0; j < cloudsXYZRGB.clouds.at(i).first.size(); j++)
                {
                    out << cloudsXYZRGB.clouds.at(i).first[j].x() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].y() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].z() << ' ';
                    out << cloudsXYZRGB.clouds.at(i).first[j].r() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].g() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].b() << '\n';
                }
            }

            file.close();
        }
    }
}

void MainWindow::on_actionSaveAllClouds_triggered()
{
    for(int i = 0; i < cloudsXYZ.names.size(); i++)
    {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::AnyFile);
        QString dir = "/Users/larong/Desktop/" + cloudsXYZ.names.at(i);
        QString strFile = dialog.getSaveFileName(this, tr("Save point cloud"), dir , tr("PLY files (*.ply)"));
        QFile file(strFile);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);

            out << "ply" << '\n' << "format ascii 1.0" << '\n';
            out << "comment Created by Cloud Visualizer " << VERSION << " (Larong)" << '\n';

            QDateTime date = QDateTime::currentDateTime();
            QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
            QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
            out << "comment Created " << formattedTime << '\n';

            out << "obj_info Generated by Cloud Visualizer!" << '\n';

            out << "element vertex " << cloudsXYZ.clouds.at(i).first.size() << '\n';

            out << "property float x" << '\n';
            out << "property float y" << '\n';
            out << "property float z" << '\n';

            out << "end_header" << '\n';

            for(unsigned long long j = 0; j < cloudsXYZ.clouds.at(i).first.size(); j++)
            {
                out << cloudsXYZ.clouds.at(i).first[j].x() << ' ' << cloudsXYZ.clouds.at(i).first[j].y() << ' ' << cloudsXYZ.clouds.at(i).first[j].z() << '\n';
            }
        }

        file.close();
    }
    for(int i = 0; i < cloudsXYZI.names.size(); i++)
    {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::AnyFile);
        QString dir = "/Users/larong/Desktop/" + cloudsXYZI.names.at(i);
        QString strFile = dialog.getSaveFileName(this, tr("Save point cloud"), dir , tr("PLY files (*.ply)"));
        QFile file(strFile);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);

            out << "ply" << '\n' << "format ascii 1.0" << '\n';
            out << "comment Created by Cloud Visualizer " << VERSION << " (Larong)" << '\n';

            QDateTime date = QDateTime::currentDateTime();
            QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
            QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
            out << "comment Created " << formattedTime << '\n';

            out << "obj_info Generated by Cloud Visualizer!" << '\n';

            out << "element vertex " << cloudsXYZI.clouds.at(i).first.size() << '\n';

            out << "property float x" << '\n';
            out << "property float y" << '\n';
            out << "property float z" << '\n';
            out << "property float scalar_intensity" << '\n';

            out << "end_header" << '\n';

            for(unsigned long long j = 0; j < cloudsXYZI.clouds.at(i).first.size(); j++)
            {
                out << cloudsXYZI.clouds.at(i).first[j].x() << ' ' << cloudsXYZI.clouds.at(i).first[j].y() << ' ' << cloudsXYZI.clouds.at(i).first[j].z() << ' ' << cloudsXYZI.clouds.at(i).first[j].intensity() << '\n';
            }
        }

        file.close();
    }
    for(int i = 0; i < cloudsXYZRGB.names.size(); i++)
    {
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::AnyFile);
        QString dir = "/Users/larong/Desktop/" + cloudsXYZRGB.names.at(i);
        QString strFile = dialog.getSaveFileName(this, tr("Save point cloud"), dir , tr("PLY files (*.ply)"));
        QFile file(strFile);

        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);

            out << "ply" << '\n' << "format ascii 1.0" << '\n';
            out << "comment Created by Cloud Visualizer " << VERSION << " (Larong)" << '\n';

            QDateTime date = QDateTime::currentDateTime();
            QString formattedTime = date.toString("dd.MM.yyyy hh:mm:ss");
            QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
            out << "comment Created " << formattedTime << '\n';

            out << "obj_info Generated by Cloud Visualizer!" << '\n';

            out << "element vertex " << cloudsXYZRGB.clouds.at(i).first.size() << '\n';

            out << "property float x" << '\n';
            out << "property float y" << '\n';
            out << "property float z" << '\n';
            out << "property uchar red" << '\n';
            out << "property uchar blue" << '\n';
            out << "property uchar green" << '\n';

            out << "end_header" << '\n';

            for(unsigned long long j = 0; j < cloudsXYZRGB.clouds.at(i).first.size(); j++)
            {
                out << cloudsXYZRGB.clouds.at(i).first[j].x() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].y() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].z() << ' ';
                out << cloudsXYZRGB.clouds.at(i).first[j].r() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].g() << ' ' << cloudsXYZRGB.clouds.at(i).first[j].b() << '\n';
            }
        }

        file.close();
    }
}

void MainWindow::on_schemeXYZ_currentIndexChanged(int index)
{
    visualizer.changeSchemeXYZ(index);
}


void MainWindow::on_schemeXYZI_currentIndexChanged(int index)
{
    visualizer.changeSchemeXYZI(index);
}


void MainWindow::on_schemeRGB_currentIndexChanged(int index)
{
    visualizer.changeSchemeXYZRGB(index);
}

void MainWindow::on_listClouds_itemSelectionChanged()
{
    ui->posX->setValue(0);
    ui->posY->setValue(0);
    ui->posZ->setValue(0);
    ui->rotX->setValue(0);
    ui->rotY->setValue(0);
    ui->rotZ->setValue(0);
    auto a = ui->listClouds->selectedItems();
    auto *tab = ui->listClouds;
    std::set<int> nowSelected;
    std::vector<int> selected, deselected;
    for(auto &i : a)
    {
        int size = nowSelected.size();
        nowSelected.insert(i->row());
        if(nowSelected.size() != size)
        {
            if(selectedRow.find(i->row()) == selectedRow.end())
            {
                selected.push_back(i->row());
            }
        }
    }
    for(auto &i : selectedRow)
    {
        if(nowSelected.find(i) == nowSelected.end())
        {
            deselected.push_back(i);
        }
    }
    for(auto &i : selected)
    {
        if(tab->item(i, 2)->text() == "XYZ")
        {
            visualizer.selectCloudXYZ(i);
        }
        else if(tab->item(i, 2)->text() == "I")
        {
            visualizer.selectCloudXYZI(i - cloudsXYZ.clouds.size());
        }
        else if(tab->item(i, 2)->text() == "RGB")
        {
            visualizer.selectCloudXYZRGB(i - cloudsXYZ.names.size() - cloudsXYZI.clouds.size());
        }
    }
    for(auto &i : deselected)
    {
        if(tab->item(i, 2)->text() == "XYZ")
        {
            visualizer.unselectCloudXYZ(i, ui->schemeXYZ->currentIndex());
        }
        else if(tab->item(i, 2)->text() == "I")
        {
            visualizer.unselectCloudXYZI(i - cloudsXYZ.clouds.size(), ui->schemeXYZI->currentIndex());
        }
        else if(tab->item(i, 2)->text() == "RGB")
        {
            visualizer.unselectCloudXYZRGB(i - cloudsXYZ.names.size() - cloudsXYZI.clouds.size(), ui->schemeRGB->currentIndex());
        }
    }
    std::cout << std::endl;
    selectedRow = nowSelected;
    visualizer.update();
}

void MainWindow::on_transButton_clicked()
{
    auto *tab = ui->listClouds;
    if(selectedRow.empty()) return;
    historyTransform.push_back({{}});
    for(auto &i : selectedRow)
    {
        if(tab->item(i, 2)->text() == "XYZ")
        {
            cloudsXYZ.clouds[i].first.transform(ui->posX->value(), ui->posY->value(), ui->posZ->value(),
                                                ui->rotX->value(), ui->rotY->value(), ui->rotZ->value(), ui->transformWay->checkState());
            visualizer.updateCloudPosXYZ(i);
            historyTransform.back().push_back({cloudsXYZ.names[i], "XYZ"});
        }
        else if(tab->item(i, 2)->text() == "I")
        {
            cloudsXYZI.clouds[i - cloudsXYZ.clouds.size()].first.transform(ui->posX->value(), ui->posY->value(), ui->posZ->value(),
                                                ui->rotX->value(), ui->rotY->value(), ui->rotZ->value());
            visualizer.updateCloudPosXYZI(i - cloudsXYZ.clouds.size());
            historyTransform.back().push_back({cloudsXYZI.names[i - cloudsXYZ.names.size()], "I"});
        }
        else if(tab->item(i, 2)->text() == "RGB")
        {
            cloudsXYZRGB.clouds[i - cloudsXYZ.names.size() - cloudsXYZI.clouds.size()].first.transform(ui->posX->value(), ui->posY->value(), ui->posZ->value(),
                                                ui->rotX->value(), ui->rotY->value(), ui->rotZ->value());
            visualizer.updateCloudPosXYZRGB(i - cloudsXYZ.names.size() - cloudsXYZI.clouds.size());
            historyTransform.back().push_back({cloudsXYZRGB.names[i - cloudsXYZ.names.size() - cloudsXYZI.clouds.size()], "RGB"});
        }
    }
}

void MainWindow::on_filterBut_clicked()
{
    auto filterType = ui->filterTypes->tabText(ui->filterTypes->currentIndex());
    auto *tab = ui->listClouds;
    if(selectedRow.empty()) return;
    historyFilter.push_back({{}});
    for(auto &i : selectedRow)
    {
        if(tab->item(i, 2)->text() == "XYZ")
        {
            if(filterType == "Border Filter")
            {
                cloudsXYZ.clouds[i].first.borderFilter(ui->borderMinX->value(), ui->borderMaxX->value(), ui->borderMinY->value(), ui->borderMaxY->value(), ui->borderMinZ->value(), ui->borderMaxZ->value(), ui->borderType->currentIndex());
            }
            visualizer.resizeCloudXYZ(i, ui->schemeXYZ->currentIndex());
            historyFilter.back().push_back({cloudsXYZ.names[i], "XYZ"});
        }
        else if(tab->item(i, 2)->text() == "I")
        {
            if(filterType == "Border Filter")
            {
                cloudsXYZI.clouds[i - cloudsXYZ.clouds.size()].first.borderFilter(ui->borderMinX->value(), ui->borderMaxX->value(), ui->borderMinY->value(), ui->borderMaxY->value(), ui->borderMinZ->value(), ui->borderMaxZ->value(), ui->borderType->currentIndex());
            }
            visualizer.resizeCloudXYZI(i - cloudsXYZ.names.size(), ui->schemeXYZI->currentIndex());
            historyFilter.back().push_back({cloudsXYZI.names[i - cloudsXYZ.clouds.size()], "I"});
        }
        else if(tab->item(i, 2)->text() == "RGB")
        {
            if(filterType == "Border Filter")
            {
                cloudsXYZRGB.clouds[i - cloudsXYZ.names.size() - cloudsXYZI.clouds.size()].first.borderFilter(ui->borderMinX->value(), ui->borderMaxX->value(), ui->borderMinY->value(), ui->borderMaxY->value(), ui->borderMinZ->value(), ui->borderMaxZ->value(), ui->borderType->currentIndex());
            }
            visualizer.resizeCloudXYZRGB(i - cloudsXYZ.names.size() - cloudsXYZI.clouds.size(), ui->schemeRGB->currentIndex());
            historyFilter.back().push_back({cloudsXYZRGB.names[i - cloudsXYZ.names.size() - cloudsXYZI.clouds.size()], "RGB"});
        }
    }
}

void MainWindow::on_undoTransButton_clicked()
{
    if(historyTransform.empty()) return;
//    std::cout << historyTransform.size() << std::endl;
    auto trans = historyTransform.back();
    historyTransform.pop_back();
    for(auto &i : trans)
    {
        if(i.second == "XYZ")
        {
            auto d = std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), i.first);
            if(d != cloudsXYZ.names.end())
            {
                int index = std::distance(cloudsXYZ.names.begin(), d);
                cloudsXYZ.clouds[index].first.undo();
                visualizer.updateCloudPosXYZ(index);
            }
        }
        else if(i.second == "I")
        {
            auto d = std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), i.first);
            if(d != cloudsXYZI.names.end())
            {
                int index = std::distance(cloudsXYZI.names.begin(), d);
                cloudsXYZI.clouds[index].first.undo();
                visualizer.updateCloudPosXYZI(index);
            }
        }
        else if(i.second == "RGB")
        {
            auto d = std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), i.first);
            if(d != cloudsXYZRGB.names.end())
            {
                int index = std::distance(cloudsXYZRGB.names.begin(), d);
                cloudsXYZRGB.clouds[index].first.undo();
                visualizer.updateCloudPosXYZRGB(index);
            }
        }
    }
}

void MainWindow::on_undoFilterBut_clicked()
{
    if(historyFilter.empty()) return;
    auto filter = historyFilter.back();
    historyFilter.pop_back();
    for(auto &cloud : filter)
    {
        if(cloud.second == "XYZ")
        {
            auto d = std::find(cloudsXYZ.names.begin(), cloudsXYZ.names.end(), cloud.first);
            if(d != cloudsXYZ.names.end())
            {
                int index = std::distance(cloudsXYZ.names.begin(), d);
                cloudsXYZ.clouds[index].first.undoFilter();
                visualizer.resizeCloudXYZ(index, ui->schemeXYZ->currentIndex());
            }
        }
        else if(cloud.second == "I")
        {
            auto d = std::find(cloudsXYZI.names.begin(), cloudsXYZI.names.end(), cloud.first);
            if(d != cloudsXYZI.names.end())
            {
                int index = std::distance(cloudsXYZI.names.begin(), d);
                cloudsXYZI.clouds[index].first.undoFilter();
                visualizer.resizeCloudXYZI(index, ui->schemeXYZI->currentIndex());
            }
        }
        else if(cloud.second == "RGB")
        {
            auto d = std::find(cloudsXYZRGB.names.begin(), cloudsXYZRGB.names.end(), cloud.first);
            if(d != cloudsXYZRGB.names.end())
            {
                int index = std::distance(cloudsXYZRGB.names.begin(), d);
                cloudsXYZRGB.clouds[index].first.undoFilter();
                visualizer.resizeCloudXYZRGB(index, ui->schemeRGB->currentIndex());
            }
        }
    }
}
