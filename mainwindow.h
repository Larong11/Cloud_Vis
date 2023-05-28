#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <fstream>
#include <QFileDialog>
#include <QLabel>
#include <QCheckBox>
#include <pointcloud.h>
#include <cloudsvisualiser.h>
#include <set>

//version of programm
#define VERSION "beta 0.7"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionOpenCloud_triggered();
    void resizeEvent(QResizeEvent* event);
    void changedStatus(QCheckBox *checkBox);
    void deleteCloud(QPushButton *pushButton);
    void on_actionSaveClouds_triggered();
    void on_actionSaveAllClouds_triggered();
    void on_schemeXYZ_currentIndexChanged(int index);
    void on_schemeXYZI_currentIndexChanged(int index);
    void on_schemeRGB_currentIndexChanged(int index);
    void on_listClouds_itemSelectionChanged();
    void on_transButton_clicked();
    void on_undoTransButton_clicked();
    void on_undoFilterBut_clicked();
    void on_filterBut_clicked();

private:
    Ui::MainWindow *ui;
    cloudsVisualiser visualizer;

    void updateClouds();

    QShortcut *selectAll, *deleteS;

    //import clouds
    insertedClouds<point> cloudsXYZ;
    insertedClouds<pointI> cloudsXYZI;
    insertedClouds<pointRGB> cloudsXYZRGB;

    //selected clouds
    std::set<int> selectedRow;

    //history
    std::vector<std::vector<std::pair<QString, QString>>> historyTransform;
    std::vector<std::vector<std::pair<QString, QString>>> historyFilter;
};
#endif // MAINWINDOW_H
