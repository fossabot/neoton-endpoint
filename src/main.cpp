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

    NTEndpoint *ep;

    parser.setApplicationDescription("Neoton Endpoint: a small player for Neoton Broadcasting system");
    parser.addHelpOption();
    parser.addOption(QCommandLineOption(QStringList() << "c" << "config", "Configuration file", "filename"));
    parser.addOption(QCommandLineOption(QStringList() << "d" << "devices", "List available playback devices and exit"));
    parser.addOption(QCommandLineOption(QStringList() << "u" << "url", "Test the player and audio device with streaming URL", "url"));
    parser.addOption(QCommandLineOption(QStringList() << "t" << "test", "Test the audio playback device with a fancy tracker music", "device"));
    parser.process(a);

    if (parser.isSet("devices"))
    {
        printf ("Listing available audio devices on this machine\n");

        QList<NTBassPlayer::AudioDevice> devices = NTBassPlayer::listDevices();

        for (int i = 0; i < devices.count(); i++)
            printf(QString("Device #%1: %2 [%3]\n").arg(i).arg(devices.at(i).name).arg(devices.at(i).driver).toLocal8Bit().data());

        return 0;
    }


    if (parser.isSet("u"))
    {
        printf ("Neoton Player: URL Test Mode\n");
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
    if (parser.isSet("t"))
    {



        printf ("       _   ____________  __________  _   __\n"
                "      / | / / ____/ __ \\/_  __/ __ \\/ | / /\n"
                "     /  |/ / __/ / / / / / / / / / /  |/ / \n"
                "    / /|  / /___/ /_/ / / / / /_/ / /|  /  \n"
                "   /_/ |_/_____/\\____/ /_/  \\____/_/ |_/ \n\n"

                "This is Neoton, a free public broadcasting system\n"
                "By Asterleen ~ https://asterleen.com\n"
                "Neoton is licensed under BSD 3-Clause license.\n\n"
                "You are using Neoton Endpoint Client. Now we perform\n"
                "an audio test: how Neoton uses your sound card.\n\n"
                );


        bool ok;
        int device = parser.value("t").toInt(&ok);

        if (!ok)
        {
            printf ("Bad device ID, run this program with -d key to see all available devices.");
            return 2;
        }

        NTBassPlayer bass;

        int currBassStatus;
        currBassStatus = bass.init(device);

        if (currBassStatus == 0)
            printf ("BASS Library Init OK\n");
        else
        {
            printf ("Could not initialize BASS Library, code #%d\n", currBassStatus);
            bass.free();
            return 1;
        }

        NTBassPlayer::AudioDevice deviceInfo = NTBassPlayer::deviceInfo(bass.device());
        printf ("Playing music using %s\n", deviceInfo.name.toUtf8().data());

        currBassStatus = bass.playTest();

        if (currBassStatus == 0)
            printf ("BASS Start OK, you should hear the sound...\n");
        else
        {
            printf ("Could not play music, code #%d\n", currBassStatus);
            bass.free();
            return 2;
        }

        // BASS_ACTIVE_PLAYING = 1 :: bass.h@597
        while(bass.bassStatus() == 1)
        {
            Sleep(100); // decreases CPU usage
        }

        printf ("Playing finished, bye!\n");
        return 0;
    }
        else
    {
        ep = new NTEndpoint (parser.value("config"));
        Q_UNUSED(ep);

        QObject::connect(&a, &QCoreApplication::aboutToQuit, ep, &NTEndpoint::deleteLater);
    }

    return a.exec();
}
