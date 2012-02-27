#include "editor.h"
#include "store.h"
#include "log.h"
#include "diffwindow.h"

QByteArray EditorThread::getData(int num, QString input)
{
    QByteArray ret;

    QString inputf = QString::number(num)+".txt";
    QString outputf = QString::number(num)+".bin";
    
    QFile input_file("/tmp/"+inputf);
    input_file.open(QFile::WriteOnly);
    QTextStream qts(&input_file);
    qts << input;
    input_file.close();

    QStringList pargs;
    pargs.push_back(inputf);
    pargs.push_back(outputf);
    pargs.append(args);
    QProcess * process = new QProcess();

    QString start = "<font color='blue'>Invoking: " + helper + " ";
    for (int loopc=0; loopc<pargs.size(); loopc++)
    {
        start += pargs[loopc];
        start += " ";
    }
    start += "</font>\n";
    emit progress_detail(start);

    process->setProcessChannelMode(QProcess::MergedChannels);
    process->start(helper, pargs);
    if (!process->waitForFinished())
    {
        emit progress_detail("<font color='red'>Failure</font>");
    }
    emit progress_detail(process->readAll());
    
    QFile shader_file("/tmp/"+outputf);
    if (!shader_file.open(QFile::ReadOnly))
    {
        emit progress_detail("Could not open "+shader_file.fileName());
    }
    else
    {
        ret = shader_file.readAll(); 
        QString bytes_got = "Got " + QString::number(ret.size()) + "bytes\n";
        emit progress_detail(bytes_got);
    }
    
    return ret;
}

void EditorThread::run()
{
    store->clear();
    if (subs.size() == 0)
    {
        subs.push_back("$");  // Null operation
    }
    
    for (int loopc=0; loopc<subs.size(); loopc++)
    {
        emit progress(QString("Processing ")+subs[loopc]);
        QString to_run = program;
        to_run.replace("$", subs[loopc]);
        store->addArray(subs[loopc], getData(loopc, to_run));
    }
    store->makeDiff();
    
    emit progress("");
}

Editor::Editor()
{
    edit = new QTextEdit;
    edit->setAcceptRichText(false);
    
    helper = new QLineEdit("./helper.sh");
    args = new QLineEdit("-v");
    subs = new QLineEdit;
    go = new QPushButton("Go");
    new_diff = new QPushButton("New diff");
    setStatusBar(new QStatusBar());
    setCentralWidget(edit);
    connect(go, SIGNAL(clicked()), this, SLOT(goPressed()));
    connect(new_diff, SIGNAL(clicked()), this, SLOT(newDiffPressed()));
    QToolBar * tb = new QToolBar();
    addToolBar(tb);
    tb->addWidget(new QLabel("Helper"));
    tb->addWidget(helper);
    tb->addWidget(new QLabel("Args"));
    tb->addWidget(args);
    tb->addWidget(new QLabel("Subs"));
    tb->addWidget(subs);
    tb->addWidget(new_diff);
    tb->addWidget(go);

    QFile test_file("shader.txt");
    if (test_file.exists())
    {
        test_file.open(QFile::ReadOnly);
        QTextStream qts(&test_file);
        edit->setText(qts.readAll());
    }
    setWindowTitle("Editor");
}

void Editor::newDiffPressed()
{
    DiffWindow * dw = new DiffWindow();
    dw->updateDisplay();
    dw->show();
}

void Editor::goPressed()
{
    EditorThread * et = new EditorThread(helper->text(),
                                         args->text().split(" "),
                                         edit->toPlainText(),
                                         subs->text().split(" "));
    
    connect(et, SIGNAL(progress(const QString &)),
            statusBar(), SLOT(showMessage(const QString &)),
            Qt::QueuedConnection);

    if (log)
    {
        connect(et, SIGNAL(progress_detail(const QString &)),
                log, SLOT(append(const QString &)),
                Qt::QueuedConnection);
    }
    
    log->clear();
    et->start();

        /* Test for floating point
    store->clear();
    float test = 0.0;
    QByteArray qba((const char *)&test, sizeof(test));
    store->addArray("0.0", qba);
    float test2 = 1.0;
    QByteArray qba2((const char *)&test2, sizeof(test2));
    store->addArray("1.0", qba2);
    float test3 = 2.0;
    QByteArray qba3((const char *)&test3, sizeof(test3));
    store->addArray("2.0", qba3);
    float test4 = -4.0;
    QByteArray qba4((const char *)&test4, sizeof(test4));
    store->addArray("-4.0", qba4);
    store->makeDiff();
        */
}
