#pragma once
#include "basicwidget/basicwidget.h"


QT_BEGIN_NAMESPACE
class MusicButton;
class QWidget;
class VideoWidget;
class QSlider;
class QMediaPlayer;
class QLabel;
QT_END_NAMESPACE


class VideoMainWidget :
	public BasicWidget
{
	Q_OBJECT
public:
	VideoMainWidget(QWidget *parent = 0);
	~VideoMainWidget();
	void setVideoUrl(const QString url);

protected:
    void closeEvent(QCloseEvent *);

private:
	void init();
	void setLayout();
	void connects();   //���е�connect������

	VideoWidget *videoWidget;
	MusicButton *play;              //���Ű�ť
	//MusicButton *fullScreen;		//ȫ����ť
	MusicButton *volume;			//��������
	QSlider *playProgessBar;		//���Ž���
	QSlider *volumeProgressBar;		//����
	QLabel *showTime;
	QString totleTime;
	QMediaPlayer *player;
	int volumeValue;
	bool isSliderPressed;
};

