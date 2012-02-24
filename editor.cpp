#include "editor.h"
#include "store.h"

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

    args.clear();
    args.push_back(inputf);
    args.push_back(outputf);
    QProcess * process = new QProcess();
    process->start(helper, args);
    process->waitForFinished();

    QFile shader_file("/tmp/"+outputf);
    shader_file.open(QFile::ReadOnly);
    ret = shader_file.readAll();
    printf("Got %d bytes\n", ret.size());
    return ret;
}

void EditorThread::run()
{
    store->clear();
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
    args = new QLineEdit("-f");
    subs = new QLineEdit;
    go = new QPushButton("Go");
    setStatusBar(new QStatusBar());
    setCentralWidget(edit);
    connect(go, SIGNAL(clicked()), this, SLOT(goPressed()));
    QToolBar * tb = new QToolBar();
    addToolBar(tb);
    tb->addWidget(new QLabel("Helper"));
    tb->addWidget(helper);
    tb->addWidget(new QLabel("Args"));
    tb->addWidget(args);
    tb->addWidget(new QLabel("Subs"));
    tb->addWidget(subs);
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

void Editor::goPressed()
{
    EditorThread * et = new EditorThread(helper->text(),
                                         args->text().split(" "),
                                         edit->toPlainText(),
                                         subs->text().split(" "));
    
    connect(et, SIGNAL(progress(const QString &)),
            statusBar(), SLOT(showMessage(const QString &)),
            Qt::QueuedConnection);
    
    et->start();
}
