#include <iostream>
#include <cstdlib>

#include <QFileDialog>
#include <QMenu>
#include <QString>
#include <QStringList>
#include <QIcon>
#include <QDebug>
#include <QSize>

#include "vlcplayer.h"


VLCPlayer::VLCPlayer(QWidget *parent)
    : QMainWindow(parent), m_row(2), m_column(2)
{
	
	ui = new Ui::VLCPlayerClass;
	m_rtsp = new RtspDialog(this);
    m_rtsp_url = new add_Rtsp_Url(this);
	m_configDlg = new ConfigureDlg(this);
	
	ui->setupUi(this);

	QMenu *file = menuBar()->addMenu("File");
	QAction *openUrl = file->addAction("Open SDP File");
    QAction *openRtsp = file->addAction("Open Rtsp Url");
    QAction *exit = file->addAction("Exit");


	QMenu *config = menuBar()->addMenu("Configure");
	QAction *layout = config->addAction("Row X Column");
	
	//创建一个Manager类，以便管理监视窗口的行列配置
	m_manager = new MonitorWindowManager(m_row, m_column, ui->container);

	ui->container->setLayout(m_manager);
	
//	for (int i = 0; i < m_row; ++i)
//	{
//		for (int j = 0; j < m_column;++j)
//		{
//			m_playerSet.append(m_manager->getItemAtPosition(i, j));
//		}
//	}

//    for (auto& player : m_playerSet)
//    {
//        connect(player->ffmpeg(), SIGNAL(GetImage(const QImage &)), player, SLOT(setImage(const QImage&)));
//        connect(player->usbCam(), SIGNAL(getUsbCamImage(const QImage &)), player, SLOT(setImage(const QImage&)));
//    }
	
	connect(ui->play, SIGNAL(clicked()), this, SLOT(Play()));
	connect(ui->replay, SIGNAL(clicked()), this, SLOT(Replay()));
	connect(ui->record, SIGNAL(clicked()), this, SLOT(Record()));
	connect(ui->stop, SIGNAL(clicked()), this, SLOT(Stop()));

	connect(exit, SIGNAL(triggered()), this, SLOT(Exit()));
	connect(openUrl, SIGNAL(triggered()), this, SLOT(OpenURL()));
    connect(openRtsp, SIGNAL(triggered()), this, SLOT(OpenRTSP()));
	connect(layout, SIGNAL(triggered()), this, SLOT(showRowColumnDlg()));
	connect(m_rtsp, SIGNAL(fileComplete(QString&, QString&,QString&)), this,SLOT(recvFile(QString&, QString&,QString&)));
	connect(m_configDlg, SIGNAL(configComplete(int, int)), this, SLOT(recvConfig(int, int)));
    connect(m_rtsp_url, SIGNAL(add_a_device(QString&,QString&)),this,SLOT(recvDevice(QString&, QString&)));

}

VLCPlayer::~VLCPlayer()
{
	delete ui;
	delete m_rtsp;
    delete m_rtsp_url;
	delete m_configDlg;
	delete m_manager;
	
}

//播放和暂停
void VLCPlayer::Play()
{
	
	for (auto &player : m_playerSet)
	{
		if (player->isActive())
		{
			if (player->isPlaying())
			{
				player->pausePlaying();
				ui->play->setIcon(QIcon(":/Resources/play.png"));

			}
			else
			{
				player->resumePlaying();
				ui->play->setIcon(QIcon(":/Resources/pause.png"));

			}
		}
	}

}

void VLCPlayer::Replay()
{
	
}

void VLCPlayer::Record()
{
	for (auto& player : m_playerSet)
	{
		if (player->isActive())
		{
			if (player->isRecording())
			{
				player->stopRecording();
				ui->record->setIcon(QIcon(":/Resources/record.png"));
			}
			else
			{
				player->startRecording();
				ui->record->setIcon(QIcon(":/Resources/offrecord.png"));
			}
		}
	}
	
}

void VLCPlayer::Exit()
{
	this->close();
}

void VLCPlayer::OpenURL()
{
	m_rtsp->show();
}

void VLCPlayer::OpenRTSP()
{
    m_rtsp_url->show();
}

void VLCPlayer::recvFile(QString& file, QString &path,QString& format)
{
    int row, col;
    if (m_manager->getAFreeWindow(row, col) < 0) return ;
    m_manager->setNextWindow(row, col);
    MonitorWindow *player = m_manager->getItemAtPosition(row, col);
    m_playerSet.append(player);
    if (!player->isActive())
    {
        ui->play->setIcon(QIcon(":/Resources/pause.png"));
        player->setUrl(file);
        player->setOutPath(path, format);
        player->startPlaying();
    }
}

void VLCPlayer::recvDevice(QString& rtsp, QString& deviceName)
{
    int row, col;
    if (m_manager->getAFreeWindow(row, col) < 0) return ;
    m_manager->setNextWindow(row, col);
    MonitorWindow *player = m_manager->getItemAtPosition(row, col);
    m_playerSet.append(player);
    if(!player->isActive())
    {
        ui->play->setIcon(QIcon(":/Resources/pause.png"));
        if(!rtsp.compare(QString("local"),Qt::CaseInsensitive ))
        {
            connect(player->usbCam(), SIGNAL(getUsbCamImage(const QImage &)), player, SLOT(setImage(const QImage&)));
            player->openUSBCam();
            player->startPlaying();
        }
        else
        {
            connect(player->ffmpeg(), SIGNAL(GetImage(const QImage &)), player, SLOT(setImage(const QImage&)));
            player->setRtspUrl(rtsp);
            player->startPlaying();
        }
    }

}

void VLCPlayer::recvConfig(int row, int column)
{
	m_row = row;
	m_column = column;

	m_manager->setMonitorLayout(m_row, m_column);

	m_playerSet.clear();

//	for (int i = 0; i < m_row; ++i)
//	{
//		for (int j = 0; j < m_column; ++j)
//		{
//			m_playerSet.append(m_manager->getItemAtPosition(i, j));
//		}
//	}

//	for (auto &player : m_playerSet)
//	{
//		connect(player->ffmpeg(), SIGNAL(GetImage(const QImage &)), player, SLOT(setImage(const QImage&)));
//	}



}

//停止，关闭监视
void VLCPlayer::Stop()
{
	
	for (auto &player : m_playerSet)
	{
		if (player->isActive())
		{
			player->stop();
		}
	}

	ui->play->setIcon(QIcon(":/Resources/play.png"));
	ui->record->setIcon(QIcon(":/Resources/record.png"));
	
	for(auto &player : m_playerSet)
	{
		player->setPixmap(QPixmap());
	}

}

void VLCPlayer::showRowColumnDlg()
{
	m_configDlg->show();
}



