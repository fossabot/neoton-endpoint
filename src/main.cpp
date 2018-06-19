#include <QCoreApplication>
#include <QCommandLineParser>
#include <QString>
#include <QStringList>
#include <QList>
#include <QUrl>

#include "ntendpoint.h"
#include "ntbassplayer.h"

/*

    This is Neoton, a public broadcasting system
       (c) Asterleen ~ https://asterleen.com
    Licensed under BSD 3-Clause License, see LICENSE

*/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QCommandLineParser parser;
    parser.setApplicationDescription("Neoton Endpoint: a small player for Neoton Broadcasting system");
    parser.addHelpOption();
    parser.addOption(QCommandLineOption(QStringList() << "c" << "config", "Configuration file", "filename"));
    parser.addOption(QCommandLineOption(QStringList() << "d" << "devices", "List available playback devices and exit"));
    parser.addOption(QCommandLineOption(QStringList() << "u" << "url", "Test the player and audio device with streaming URL", "url"));
    parser.addOption(QCommandLineOption(QStringList() << "t" << "test", "Test the audio playback device with a fancy tracker music", "device"));
    parser.process(a);

    if (parser.isSet("devices"))
    {
        QList<NTBassPlayer::AudioDevice> devices = NTBassPlayer::listDevices();

        for (int i = 0; i < devices.count(); i++)
            printf(QString("Device #%1: %2 [%3]\n").arg(i).arg(devices.at(i).name).arg(devices.at(i).driver).toLocal8Bit().data());

        return 0;
    }

    if (parser.isSet("t"))
    {
        printf ("Neoton Player: Test Mode\n");
        printf ("Playing %s using default audio device...\n", parser.value("test").toUtf8().data());

        NTBassPlayer bass;
        bass.setUrl(QUrl(parser.value("t")));

        int currBassStatus;
        currBassStatus = bass.init();

        if (currBassStatus == 0)
            printf ("BASS Init OK\n");
        else
        {
            printf ("Could not initialize BASS, code #%d\n", currBassStatus);
            bass.free();
            return 1;
        }

        currBassStatus = bass.playUrl();

        if (currBassStatus == 0)
            printf ("BASS Start OK, you should hear the sound...\n");
        else
        {
            printf ("Could not play the stream, code #%d\n", currBassStatus);
            bass.free();
            return 2;
        }
    }
        else
    {
        NTEndpoint ep(parser.value("config"));
        Q_UNUSED(ep);
    }

    return a.exec();
}