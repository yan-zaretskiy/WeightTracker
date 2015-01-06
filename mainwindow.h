#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;

namespace weighttracker {

class WTWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    bool open();
    bool save();
    bool saveAs();
    void openRecentFile();
    void clearRecentFiles();
    void weightTableModified();

private:
    void createActions();
    void createMenus();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();

    QString strippedName(const QString &fullFileName);

    WTWidget* wtwidget_;
    QHash<QString,QAction*> actions_;
    static const int maxRecentFiles = 5;
    QAction *recentFileActions_[maxRecentFiles];
    QStringList recentFiles_;
    QString curFile_;
};

}

#endif // MAINWINDOW_H
