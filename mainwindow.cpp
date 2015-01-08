#include <QtWidgets>

#include "mainwindow.h"
#include "wtwidget.h"

namespace weighttracker {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    createActions();
    createMenus();

    setWindowIcon(QIcon(":/images/chart.png"));

    readSettings();
    setCurrentFile("");
    wtwidget_ = new WtWidget(this);
    setCentralWidget(wtwidget_);
}


MainWindow::~MainWindow() { }


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue())
    {
        writeSettings();
        event->accept();
    }
    else
        event->ignore();
}


void MainWindow::newFile()
{
    if (okToContinue())
    {
        emit clearModelRequested();
        setCurrentFile("");
    }
}


bool MainWindow::open()
{
    bool result = false;
    if (okToContinue())
    {
        QString fileName = QFileDialog::getOpenFileName(this,
                                   tr("Open Weight Table"), ".",
                                   tr("Weight Table files (*.wt)"));
        result = !fileName.isEmpty() && loadFile(fileName);
    }
    return result;
}


bool MainWindow::save()
{
    return curFile_.isEmpty() ? saveAs() : saveFile(curFile_);
}


bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                               tr("Save Weight Table"), ".",
                               tr("Weight Table files (*.wt)"));

    return !fileName.isEmpty() && saveFile(fileName);
}


void MainWindow::openRecentFile()
{
    if (okToContinue())
    {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            loadFile(action->data().toString());
    }
}


void MainWindow::clearRecentFiles()
{
    recentFiles_.clear();
    updateRecentFileActions();
}


void MainWindow::weightTableModified()
{
    setWindowModified(true);
}


void MainWindow::createMenus()
{
    QMenu *file = menuBar()->addMenu(tr("&File"));
    file->addAction(actions_.value("new"));
    file->addAction(actions_.value("open"));
    file->addAction(actions_.value("save"));
    file->addAction(actions_.value("saveAs"));

    QMenu *recentFileMenu_ = file->addMenu(tr("&Recent Files"));
    recentFileMenu_->setObjectName("recentFilesMenu");
    for (int i = 0; i < maxRecentFiles; ++i)
        recentFileMenu_->addAction(recentFileActions_[i]);
    recentFileMenu_->addSeparator();
    recentFileMenu_->addAction(actions_.value("clearRecent"));

    file->addAction(actions_.value("exit"));
}


void MainWindow::readSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Yan Zaretskiy", "Weight Tracker");
    //restoreGeometry(settings.value("geometry").toByteArray()); // gotta figure out the right defaults first, needs to be uncommented eventually
    recentFiles_ = settings.value("recentFiles").toStringList();
    updateRecentFileActions();
}


void MainWindow::writeSettings()
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Yan Zaretskiy", "Weight Tracker");
    //settings.setValue("geometry", saveGeometry());
    settings.setValue("recentFiles", recentFiles_);
}


bool MainWindow::okToContinue()
{
    if (isWindowModified())
    {
        int r = QMessageBox::warning(this, tr("Weight Tracker"),
                        tr("The document has been modified.\n"
                           "Do you want to save your changes?"),
                        QMessageBox::Yes | QMessageBox::No
                        | QMessageBox::Cancel, QMessageBox::No);
        if (r == QMessageBox::Yes)
            return save();
        else if (r == QMessageBox::Cancel)
            return false;
    }
    return true;
}


bool MainWindow::loadFile(const QString &fileName)
{
    bool isReadOK = wtwidget_->readFile(fileName);
    if (isReadOK) setCurrentFile(fileName);

    return isReadOK;
}


bool MainWindow::saveFile(const QString &fileName)
{
    bool isWriteOK = wtwidget_->writeFile(fileName);
    if (isWriteOK) setCurrentFile(fileName);

    return isWriteOK;
}


void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile_ = fileName;
    setWindowModified(false);

    QString shownName = tr("Untitled");
    if (!curFile_.isEmpty())
    {
        shownName = strippedName(curFile_);
        recentFiles_.removeAll(curFile_);
        recentFiles_.prepend(curFile_);
        updateRecentFileActions();
    }

    setWindowTitle(tr("%1[*] - %2").arg(shownName)
                   .arg(tr("Weight Tracker")));
}


void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator iter(recentFiles_);
    while (iter.hasNext())
    {
        if (!QFile::exists(iter.next()))
            iter.remove();
    }

    for (int i = 0; i < maxRecentFiles; ++i)
    {
        if (i < recentFiles_.count())
        {
            QString text = tr("&%1 %2").arg(i + 1).arg(strippedName(recentFiles_[i]));
            recentFileActions_[i]->setText(text);
            recentFileActions_[i]->setData(recentFiles_[i]);
            recentFileActions_[i]->setVisible(true);
        }
        else
            recentFileActions_[i]->setVisible(false);
    }
    QMenu *rfMenu = this->findChild<QMenu *>("recentFilesMenu");
    rfMenu->setEnabled(!recentFiles_.isEmpty());
}


void MainWindow::createActions()
{
    QAction* newAction = new QAction(tr("&New"), this);
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new weight table"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));
    actions_["new"] = newAction;

    QAction* openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open an existing weight table"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    actions_["open"] = openAction;

    QAction* saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save the weight table to disk"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    actions_["save"] = saveAction;

    QAction* saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setStatusTip(tr("Save the weight table under a new name"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    actions_["saveAs"] = saveAsAction;

    for (int i = 0; i < maxRecentFiles; ++i)
    {
        recentFileActions_[i] = new QAction(this);
        recentFileActions_[i]->setVisible(false);
        connect(recentFileActions_[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    QAction* clearRecentAction = new QAction(tr("&Clear"), this);
    connect(clearRecentAction, SIGNAL(triggered()),this, SLOT(clearRecentFiles()));
    actions_["clearRecent"] = clearRecentAction;

    QAction* exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    actions_["exit"] = exitAction;
}


QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}


}
