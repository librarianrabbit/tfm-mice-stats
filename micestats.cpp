#include "micestats.h"
#include "ui_micestats.h"

#include <QtNetwork/QNetworkRequest>
#include <QtXml/qdom.h>
#include <QDesktopServices>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>

#include "micenamevalidator.h"

#include <QDebug>

bool isStdTr = false;
QString stdTr;

bool ldEn = false;
bool isEn = false;

MiceStats::MiceStats(QWidget *parent) :
    QMainWindow(parent, Qt::Window | Qt::MSWindowsFixedSizeDialogHint),
    ui(new Ui::MiceStats)
{
    ten = new QTranslator;
    ldEn = ten->load(":/translations/micestats_en.qm");

    QString fname(qApp->applicationFilePath());

    fname.replace(QRegExp(".exe$", Qt::CaseInsensitive), ".ini");

    if (!QFile(fname).exists())
    {
        QString dir = qApp->applicationDirPath();
        dir.append("/Standalone.ini");

        if (QFile(dir).exists())
        {
            QSettings st(dir, QSettings::IniFormat);

            stdTr = st.value("translation", "pt").toString().toLower();
            isStdTr = true;
        }
    }

    settings = new QSettings(fname, QSettings::IniFormat);

    if (isStdTr)
    {
        settings->setValue("translation", stdTr);
    }

    if (settings->value("translation", "pt").toString().toLower() == "en")
    {
        if (ldEn)
        {
            qApp->installTranslator(ten);
            isEn = true;
        }
    }

    ui->setupUi(this);

    if (isEn)
    {
        ui->actionEnglish->setChecked(true);
        ui->actionPortugu_s->setChecked(false);
    }
    else
    {
        ui->actionEnglish->setChecked(false);
        ui->actionPortugu_s->setChecked(true);
    }

    ui->statusBar->setSizeGripEnabled(false);
    ui->statusBar->showMessage(tr("Pronto."));

    manager = new QNetworkAccessManager(this);

    ui->lineEdit->setValidator(new MiceNameValidator(ui->lineEdit));

    connect(ui->pushButton, SIGNAL(clicked()), SLOT(getData()));
    connect(ui->lineEdit, SIGNAL(returnPressed()), SLOT(getData()));
    connect(this->manager, SIGNAL(finished(QNetworkReply*)), SLOT(gotData(QNetworkReply*)));

    connect(ui->wikilink, SIGNAL(linkActivated(QString)), this, SLOT(openLink(QString)));
    connect(ui->apilink, SIGNAL(linkActivated(QString)), this, SLOT(openLink(QString)));

    connect(ui->actionEnglish, SIGNAL(triggered()), this, SLOT(goEn()));
    connect(ui->actionPortugu_s, SIGNAL(triggered()), this, SLOT(goPt()));
    connect(ui->actionSalvar_xml, SIGNAL(triggered()), this, SLOT(saveXml()));
    connect(ui->actionSobre, SIGNAL(triggered()), this, SLOT(showAbout()));
    connect(ui->actionSobre_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MiceStats::showAbout()
{
    QMessageBox::about(this, "MiceStats v1.0.1", tr("<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:10pt; font-weight:600;\">MiceStats v1.0.1</span></p><br/><span style=\" font-size:11pt; font-weight:600; vertical-align:super;\">by Rafaelfsilva</span>"));
}

void MiceStats::goEn()
{
    if (!isEn)
    {
        if (ten && ldEn)
        {
            isEn = true;

            qApp->installTranslator(ten);
            settings->setValue("translation", "en");

            ui->actionEnglish->setChecked(true);
            ui->actionPortugu_s->setChecked(false);
        }
        else
        {
            isEn = false;

            settings->setValue("translation", "pt");

            ui->actionEnglish->setChecked(false);
            ui->actionPortugu_s->setChecked(true);
        }

        ui->retranslateUi(this);
    }
}

void MiceStats::goPt()
{
    if (isEn && ldEn)
    {
        isEn = false;

        qApp->removeTranslator(ten);
        settings->setValue("translation", "pt");

        ui->actionEnglish->setChecked(false);
        ui->actionPortugu_s->setChecked(true);

        ui->retranslateUi(this);
    }
}

void MiceStats::saveXml()
{
    if (this->last_xml.isEmpty()) return;

    QString fn = QFileDialog::getSaveFileName(this, QString(), QString(),  tr("Arquivos XML (*.xml)"));

    if (!fn.isEmpty())
    {
        QFile fop(fn);

        if (fop.open(QFile::WriteOnly))
        {
            fop.write(this->last_xml.toAscii());
            fop.close();
        }
    }
}

void MiceStats::openLink(const QString& link)
{
    QDesktopServices::openUrl(QUrl(link));
}

void MiceStats::parse(const QString& data)
{
    QDomDocument doc = QDomDocument("MiceStat");
    doc.setContent(data);

    QDomElement e = doc.documentElement();

    if (e.nodeName() != "mice")
    {
        ui->statusBar->showMessage(tr("Arquivo de informações inválido."));

        return;
    }

    QDomNode node = e.firstChild();

    if (node.nodeName() == "error")
    {
        this->clear();

        QString uk = node.toElement().text();
        ui->name->setText(uk);

        return;
    }
    else if (node.nodeName() == "name")
    {
        ui->name->setText(node.toElement().text());
    }

    node = node.nextSibling();
    ui->tribe->setText(node.toElement().text());

    node = node.nextSibling();
    int rounds = node.toElement().text().toInt();
    ui->rounds->setText(node.toElement().text());

    node = node.nextSibling();
    int cheese = node.toElement().text().toInt();
    ui->cheese->setText(node.toElement().text());

    node = node.nextSibling();
    int first = node.toElement().text().toInt();
    ui->cheeseFirst->setText(node.toElement().text());

    node = node.nextSibling();
    int saves = node.toElement().text().toInt();
    ui->saves->setText(node.toElement().text());

    node = node.nextSibling();
    int cheeseShaman = node.toElement().text().toInt();
    ui->cheeseShaman->setText(node.toElement().text());

    node = node.nextSibling();
    int gold = node.toElement().text().toInt();
    ui->gold->setText(node.toElement().text());

    node = node.nextSibling();
    ui->title->setText(titles.getTitle(node.toElement().text().toInt()));

    if (rounds == 0)
    {
        ui->cheesePercent->setText("0%");
        ui->cheeseFirstPercent->setText("0%");
        ui->savesPercent->setText("0%");
        ui->cheeseShamanPercent->setText("0%");
        ui->goldPercent->setText("0%");

        return;
    }

    double cheesePercent = ((double)cheese / (double)rounds) * 100.0;
    double firstPercent = ((double)first / (double)rounds) * 100.0;
    double savesPercent = ((double)saves / (double)rounds) * 100.0;
    double shamanPercent = ((double)cheeseShaman / (double)rounds) * 100.0;
    double goldPercent = ((double)gold / (double)rounds) * 100.0;

    QString tmp;

    ui->cheesePercent->setText(tmp.sprintf("%0.2f%%", cheesePercent));
    ui->cheeseFirstPercent->setText(tmp.sprintf("%0.2f%%", firstPercent));
    ui->savesPercent->setText(tmp.sprintf("%0.2f%%", savesPercent));
    ui->cheeseShamanPercent->setText(tmp.sprintf("%0.2f%%", shamanPercent));
    ui->goldPercent->setText(tmp.sprintf("%0.2f%%", goldPercent));
}

void MiceStats::clear()
{
    ui->name->clear();
    ui->title->clear();
    ui->tribe->clear();
    ui->rounds->clear();
    ui->cheese->clear();
    ui->cheesePercent->clear();
    ui->cheeseFirst->clear();
    ui->cheeseFirstPercent->clear();
    ui->saves->clear();
    ui->savesPercent->clear();
    ui->gold->clear();
    ui->goldPercent->clear();
    ui->cheeseShaman->clear();
    ui->cheeseShamanPercent->clear();
}

QString MiceStats::link(const QString& micename)
{
    return QString("http://statapi.transformice.com/miceinfo.xml?n=").append(micename);
}

void MiceStats::getData()
{
    QString micename = ui->lineEdit->text();
    if (micename.isEmpty()) return;

    ui->statusBar->showMessage(tr("Carregando informações para ").append(micename));

    QString url = link(micename);

    QNetworkRequest request = QNetworkRequest(QUrl(url));
    manager->get(request);
}

void MiceStats::gotData(QNetworkReply* reply)
{
    if (reply->error())
    {
        ui->statusBar->showMessage(tr("Erro ao baixar informações."));
    }
    else
    {
        QString xmldata = reply->readAll();
        ui->statusBar->showMessage(tr("Informações carregadas."));

        last_xml = xmldata;

        parse(xmldata);
    }
}

MiceStats::~MiceStats()
{
    delete ui;
}
