#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QVector>
#include <math.h>
#include <limits>
#include <algorithm>
#include "QFile"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QVector<double> GetA() { return a; }
    QVector<double> GetB() { return b; }
    QVector<double> GetC() { return c; }
    QVector<double> GetD() { return d; }
    QVector<double> GetX() { return x; }
    QVector<double> GetF_x() { return f_x; }
    void SetX(QVector<double> vec) { x = vec; }
    void SetF_x(QVector<double> vec) { f_x = vec; }
    int GetPoints() { return points; }
    void CreateVectors(QVector<double>& x, QVector<double>& f_x);
    QVector<double> CreateAlpha(QVector<double> a, QVector<double>& alpha, QVector<double> delta_x, int points);
    QVector<double> CreateC(QVector<double>& c, QVector<double> alpha, QVector<double> delta_x, int points);
    QVector<double> CreateB(QVector<double>& b, QVector<double> c, QVector<double> delta_x, QVector<double> delta_y, int points);
    QVector<double> CreateD(QVector<double>& d, QVector<double> c, QVector<double> delta_x, int points);
    void Interpolate(double value, double& spline);
private:
    Ui::MainWindow *ui;
    QVector<double> a;
    QVector<double> b;
    QVector<double> c;
    QVector<double> d;
    QVector<double> x;
    QVector<double> f_x;
    int points;
};
#endif // MAINWINDOW_H
