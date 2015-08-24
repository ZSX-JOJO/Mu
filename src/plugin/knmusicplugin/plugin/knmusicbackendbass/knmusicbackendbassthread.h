/*
 * Copyright (C) Kreogist Dev Team
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifndef KNMUSICBACKENDBASSTHREAD_H
#define KNMUSICBACKENDBASSTHREAD_H

#include "bass.h"

#include "knmusicutil.h"

#include "knmusicstandardbackendthread.h"

using namespace MusicUtil;

/*!
 * \brief The KNMusicBackendBassThread class is the standard playing thread for
 * the bass backend. It should be constructed only in the KNMusicBackendBass.
 */
class KNMusicBackendBassThread : public KNMusicStandardBackendThread
{
    Q_OBJECT
public:
    /*!
     * \brief Construct a KNMusicBackendBassThread object.
     * \param parent The KNMusicBackendBass object.
     */
    explicit KNMusicBackendBassThread(QObject *parent = 0);
    ~KNMusicBackendBassThread();

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::loadFile().
     */
    bool loadFile(const QString &filePath) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::reset().
     */
    void reset() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::stop().
     */
    void stop() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::play().
     */
    void play() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::pause().
     */
    void pause() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::volume().
     */
    int volume() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::duration().
     * \return
     */
    qint64 duration() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::position().
     */
    qint64 position() Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::state().
     */
    int state() const Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::setPlaySection().
     */
    void setPlaySection(const qint64 &start=-1,
                        const qint64 &duration=-1) Q_DECL_OVERRIDE;

signals:
    /*!
     * \brief This signal is used only for threadReachesEnd(). It's used for
     * avoid the 'QObject::killTimer: Timers cannot be stopped from another
     * thread' bug.
     */
    void reachesFinished();

public slots:
    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::setVolume().
     */
    void setVolume(const int &volume) Q_DECL_OVERRIDE;

    /*!
     * \brief Reimplemented from KNMusicStandardBackendThread::setPosition().
     */
    void setPosition(const qint64 &position) Q_DECL_OVERRIDE;

    /*!
     * \brief Set the flags when using at creating a stream.
     * \param channelFlags The new channel flags.
     */
    void setCreateFlags(const DWORD &channelFlags);

private slots:
    void checkPosition();

private:
    static void CALLBACK threadReachesEnd(HSYNC handle,
                                          DWORD channel,
                                          DWORD data,
                                          void *user);
    inline void finishPlaying();
    inline void resetChannelInformation();
    inline void setPlayingState(const int &state);
    inline void setChannelSyncs();
    inline void removeChannelSyncs();
    inline qint64 getChannelPosition()
    {
        //Get the current position
        return (qint64)
                (BASS_ChannelBytes2Seconds(
                     m_channel,
                     BASS_ChannelGetPosition(m_channel, BASS_POS_BYTE))
                 //Translate the unit from second to msecond, remove the start
                 //position msecond.
                 *1000)-m_startPosition;
    }
    inline qreal getChannelVolume()
    {
        Q_ASSERT(m_channel);
        //Initial a cache.
        float channelVolume;
        //Get the volume to the cache.
        BASS_ChannelGetAttribute(m_channel, BASS_ATTRIB_VOL, &channelVolume);
        //Give back the channel volume.
        return channelVolume;
    }
    inline void freeChannel()
    {
        //Check if the channel is not null.
        if(m_channel)
        {
            //Free the streams or the music.
            if(!BASS_StreamFree(m_channel))
            {
                BASS_MusicFree(m_channel);
            }
        }
    }

    //Channel data.
    QString m_filePath;
    DWORD m_channel;
    DWORD m_channelFlags;

    //Channel information.
    qint64 m_totalDuration,
           m_duration,
           m_startPosition,
           m_endPosition;
    int m_state;
    qreal m_volume;

    //Updater.
    QTimer *m_positionUpdater;

    //Sync Handlers.
    QList<HSYNC> m_syncHandlers;
};

#endif // KNMUSICBACKENDBASSTHREAD_H
