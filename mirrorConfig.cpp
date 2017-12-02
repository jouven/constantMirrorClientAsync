#include "mirrorConfig.hpp"

#include "updateServer.hpp"
#include "fileListRequestClientThread.hpp"
#include "downloadClient.hpp"

#include "signalso/signal.hpp"

#include "essentialQtso/essentialQt.hpp"
#include "fileHashQtso/fileHashQt.hpp"
#include "qmutexUMapQtso/qmutexUMapQt.hpp"
#include "sslUtilsso/sslUtils.hpp"
#include "threadedFunctionQtso/threadedFunctionQt.hpp"

#include "comuso/practicalTemplates.hpp"

#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QHostAddress>
#include <QHostInfo>
#include <QCommandLineParser>
#include <QRegExp>
#include <QSslConfiguration>
#include <QSslKey>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSet>

#include <boost/math/common_factor.hpp>

int_fast64_t generateId_f()
{
    static int_fast64_t rootId(0);
    rootId = rootId + 1;
    return rootId;
}

QString mirrorConfigSourceDestinationMapping_c::sourcePath_f() const
{
    return sourcePath_pri;
}

//QString mirrorConfigSourceDestinationMapping_c::destinationPath_f() const
//{
//    return destinationPath_pri;
//}

quint16 mirrorConfigSourceDestinationMapping_c::sourceRequestPort_f() const
{
    return sourceRequestPort_pri;
}

//quint16 mirrorConfigSourceDestinationMapping_c::sourceDownloadPort_f() const
//{
//    return sourceDownloadPort_pri;
//}

//qint64 mirrorConfigSourceDestinationMapping_c::localCheckIntervalMilliseconds_f() const
//{
//    return localCheckIntervalMilliseconds_pri;
//}

//qint64 mirrorConfigSourceDestinationMapping_c::remoteCheckIntervalMilliseconds_f() const
//{
//    return remoteCheckIntervalMilliseconds_pri;
//}

int_fast32_t mirrorConfigSourceDestinationMapping_c::currentDownloadCount_f() const
{
    return currentDownloadCount_pri;
}

bool mirrorConfigSourceDestinationMapping_c::isValid_f() const
{
    return isValid_pri;
}

//std::unordered_map<std::string, fileStatus_s> mirrorConfigSourceDestinationMapping_c::getSourceFileStatusUMAP_f()
//{
//    QMutexLocker locker(&localHashGenerationMutex_pri);
//    return localFileStatusUMAP_pri;
//}

//QString mirrorConfigSourceDestinationMapping_c::getIncludeDirectoriesWithFileX_f() const
//{
//    return includeDirectoriesWithFileX_pri;
//}

qint64 mirrorConfigSourceDestinationMapping_c::gcdWaitMilliseconds_f() const
{
    return gcdWaitMilliseconds_pri;
}

QHostAddress mirrorConfigSourceDestinationMapping_c::sourceAddress_f() const
{
    return sourceAddress_pri;
}

bool mirrorConfigSourceDestinationMapping_c::remoteTriedOnce_f() const
{
    return remoteTriedOnce_pri;
}

void mirrorConfigSourceDestinationMapping_c::printRemoteFileList_f() const
{
    if (initialRemoteScanSet_pri)
    {
        fileStatusArrayPlusHostInfo_s objToSerialize(remoteFileStatusUMAP_pri, sourceAddressStr_pri, sourceRequestPort_pri);
        QJsonObject jsonObjectTmp;
        objToSerialize.write_f(jsonObjectTmp);

        QJsonDocument jsonDocTmp(jsonObjectTmp);

        QOUT_TS(jsonDocTmp.toJson(QJsonDocument::JsonFormat::Indented));
    }
}

mirrorConfigSourceDestinationMapping_c::mirrorConfigSourceDestinationMapping_c()
    : id_pri(generateId_f())
{
}

void mirrorConfigSourceDestinationMapping_c::read_f(const QJsonObject &json)
{
    originalSourcePath_pri = json["sourcePath"].toString();
    sourcePath_pri = QDir::fromNativeSeparators(originalSourcePath_pri);
    sourceAddressStr_pri = json["sourceAddress"].toString();
    sourceRequestPort_pri = json["sourceRequestPort"].toInt();
    sourceDownloadPort_pri = json["sourceDownloadPort"].toInt();
    originalDestinationPath_pri = json["destinationPath"].toString();
    if (originalDestinationPath_pri.isEmpty())
    {
        destinationPath_pri = sourcePath_pri;
    }
    else
    {
        destinationPath_pri = QDir::fromNativeSeparators(originalDestinationPath_pri);
    }
    QJsonArray jsonArrayFilenameFiltersTmp(json["filenameFilters"].toArray());
    if (not jsonArrayFilenameFiltersTmp.isEmpty())
    {
        filenameFilters_pri.reserve(jsonArrayFilenameFiltersTmp.size());
        for (const auto& jsonArrayItem_ite_con : jsonArrayFilenameFiltersTmp)
        {
            filenameFilters_pri.append(jsonArrayItem_ite_con.toString());
            //qout_glo << "jsonArrayItem_ite_con.toString() " << jsonArrayItem_ite_con.toString() << endl;
        }
    }
    includeSubdirectories_pri = json["includeSubdirectories"].toBool(true);
    includeDirectoriesWithFileX_pri = json["includeDirectoriesWithFileX"].toString();
    noSubdirectoriesInDestination_pri = json["noSubdirectoriesInDestination"].toBool(false);
    if (not json["localCheckIntervalMilliseconds"].isUndefined())
    {
        localCheckIntervalMilliseconds_pri = json["localCheckIntervalMilliseconds"].toInt(1000);
    }
    if (not json["remoteCheckIntervalMilliseconds"].isUndefined())
    {
        remoteCheckIntervalMilliseconds_pri = json["remoteCheckIntervalMilliseconds"].toInt(15000);
    }
    syncDeletions_pri = json["syncDeletions"].toBool(true);
    deleteThenCopy_pri = json["deleteThenCopy"].toBool(false);
}

void mirrorConfigSourceDestinationMapping_c::write_f(QJsonObject &json) const
{
    json["sourcePath"] = sourcePath_pri;
    json["sourceAddress"] = sourceAddressStr_pri;
    json["sourceRequestPort"] = sourceRequestPort_pri;
    json["sourceDownloadPort"] = sourceDownloadPort_pri;
    json["destinationPath"] = destinationPath_pri;
    if (not filenameFilters_pri.isEmpty())
    {
        QJsonArray jsonArrayFilenameFiltersTmp;
        for (const auto& filenameFilter_ite_con : filenameFilters_pri)
        {
            jsonArrayFilenameFiltersTmp.append(QJsonValue(filenameFilter_ite_con));
        }
        json["filenameFilters"] = jsonArrayFilenameFiltersTmp;
    }
    json["includeSubdirectories"] = includeSubdirectories_pri;
    json["includeDirectoriesWithFileX"] = includeDirectoriesWithFileX_pri;
    json["noSubdirectoriesInDestination"] = noSubdirectoriesInDestination_pri;
    json["localCheckIntervalMilliseconds"] = localCheckIntervalMilliseconds_pri;
    json["remoteCheckIntervalMilliseconds"] = remoteCheckIntervalMilliseconds_pri;
    json["syncDeletions"] = syncDeletions_pri;
    json["deleteThenCopy"] = deleteThenCopy_pri;
}

void mirrorConfigSourceDestinationMapping_c::setRemoteHasUpdated_f()
{
    remoteHasUpdated_pri = true;
}

//void mirrorConfigSourceDestinationMapping_c::localScan_f()
//{
//    if (not eines::signal::isRunning_f())
//    {
//        return;
//    }
//    if (currentDownloadCount_pri > 0)
//    {
//        return;
//    }
//    //if the last checked interval + interval time is greater than the current time --> skip
//    if ((localLastCheckedIntervalMilliseconds_pri + localCheckIntervalMilliseconds_pri) > std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count())
//    {
//        //do nothing
//    }
//    else
//    {
//        localLastCheckedIntervalMilliseconds_pri = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

//        bool anyFileChangedTmp(false);
//        QFileInfo destinationTmp(destinationPath_pri);
//        QMutexLocker Mlocker(getAddMutex_f(QString::number(id_pri).toStdString() + "local"));
//        //getAddMutex_f(QString::number(id_pri).toStdString() + "local")->lock();
//        if (destinationTmp.exists())
//        {
//            //case single file
//            if (destinationTmp.isFile())
//            {
//                anyFileChangedTmp = hashFileInUMAP_f(
//                            localFileStatusUMAP_pri
//                            , destinationTmp
//                            );
//                //this is going to have only one item
//                for (const auto& localItem_ite_con: localFileStatusUMAP_pri)
//                {
//                    localHash_pri = localItem_ite_con.second.hash_pub;
//                    localLastModificationTime_pri = localItem_ite_con.second.fileLastModificationDatetime_pub;
//                }
//            }

//            //case directory (0 or more files)
//            if (destinationTmp.isDir())
//            {
//                bool includeSubdirectoriesTmp(includeSubdirectories_pri);
//                //because all the files are placed in the destination root directory
//                //so locally it's the same effect as includeSubdirectories_pri = false
//                if (noSubdirectoriesInDestination_pri)
//                {
//                    includeSubdirectoriesTmp = false;
//                }
//                anyFileChangedTmp = hashDirectoryInUMAP_f(
//                            localFileStatusUMAP_pri
//                            , destinationTmp
//                            , filenameFilters_pri
//                            , includeSubdirectoriesTmp
//                            , includeDirectoriesWithFileX_pri
//                            );
//            }
//        }

//        //remove non-existing keys
//        std::vector<std::string> localFileKeysToRemove;
//        for (auto& localFileStatus_ite : localFileStatusUMAP_pri)
//        {
//            if (not localFileStatus_ite.second.iterated_pub)
//            {
//                localFileKeysToRemove.emplace_back(localFileStatus_ite.first);
//            }
//            else
//            {
//                localFileStatus_ite.second.iterated_pub = false;
//            }
//        }

//        for (const auto& localFileKey_ite_con : localFileKeysToRemove)
//        {
//            auto removeResult(localFileStatusUMAP_pri.erase(localFileKey_ite_con));
//            if (removeResult > 0)
//            {
//                anyFileChangedTmp = true;
//            }
//        }
//        //getAddMutex_f(QString::number(id_pri).toStdString() + "local")->unlock();
//        if (anyFileChangedTmp)
//        {
//            compareRequired_pri = true;
//        }
//        if (not initialLocalScanSet_pri)
//        {
//            initialLocalScanSet_pri = true;
//        }
//    }
//}

void mirrorConfigSourceDestinationMapping_c::checkRemoteFiles_f()
{
    if (not eines::signal::isRunning_f())
    {
        return;
    }
    if (remoteScanThreadExists_pri)
    {
        return;
    }

#ifdef DEBUGJOUVEN
    //QOUT_TS("(mirrorConfig_c::checkRemoteFiles_f()) remoteHasUpdated_pri " << remoteHasUpdated_pri << endl);
#endif
    if (not remoteHasUpdated_pri)
    {
        //if the last checked interval + interval time is greater than the current time --> skip
        if ((remoteLastCheckedIntervalMilliseconds_pri + remoteCheckIntervalMilliseconds_pri) > std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count())
        {
            return;
        }
    }
    else
    {
#ifdef DEBUGJOUVEN
    //QOUT_TS("(mirrorConfig_c::checkRemoteFiles_f()) remoteHasUpdated_pri = false; " << endl);
#endif
        remoteHasUpdated_pri = false;
    }

    remoteLastCheckedIntervalMilliseconds_pri = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
#ifdef DEBUGJOUVEN
    //QOUT_TS("(mirrorConfig_c::checkRemoteFiles_f()) sourceRequestPort_pri " << sourceRequestPort_pri << endl);
#endif
    fileListRequestClientThread_c* fileListRequestThreadTmp = new fileListRequestClientThread_c(
                sourceAddress_pri
                , sourceRequestPort_pri
                , &destinationJSONByteArray_pri
                , qApp
    );
    //this is not threaded?, maybe lamdas are, only the requesting/reading the json data from the server
    QObject::connect(fileListRequestThreadTmp, &QThread::finished, [this]
    {
        QJsonDocument jsonDocumentTmp(QJsonDocument::fromJson(destinationJSONByteArray_pri));
//#ifdef DEBUGJOUVEN
//        QOUT_TS("(QObject::connect(fileListRequestThreadTmp, &QThread::finished, [this] destinationJSONByteArray_pri " << destinationJSONByteArray_pri << endl);
//#endif
        destinationJSONByteArray_pri.clear();
        if (jsonDocumentTmp.isNull())
        {
//#ifdef DEBUGJOUVEN
            QOUT_TS("File list request error, empty json document" << endl);
//#endif
        }
        else
        {
            //bool remoteUMAPChangedTmp(false);
            fileStatusArray_s remoteFileStatusArrayTmp;
            remoteFileStatusArrayTmp.read_f(jsonDocumentTmp.object());
            QMutexLocker Mlocker(getAddMutex_f(QString::number(id_pri).toStdString() + "remote"));
            //getAddMutex_f(QString::number(id_pri).toStdString() + "remote")->lock();
            //put all the existing remote item to not iterated
            for (auto& remoteItem_ite : remoteFileStatusUMAP_pri)
            {
                remoteItem_ite.second.iterated_pub = false;
            }
            if (remoteFileStatusArrayTmp.fileStatusVector_pub.empty())
            {
                //YES, EVERYTHING WILL GET DELETED, but that's how it works, it syncs the remote "empty" side
                //remoteUMAPChangedTmp = true;
                //server returning an empty file list, not an error per se
#ifdef DEBUGJOUVEN
                QOUT_TS("(mirrorConfig_c::checkRemoteFiles_f()) fileStatusArrayTmp.fileStatusVector_pub.empty()" << endl);
#endif
            }
            else
            //if something was returned by the file list request
            {
                //set the filename filter if necessary
                std::vector<QRegExp> filtersTmp;
                if (not filenameFilters_pri.isEmpty())
                {
                    filtersTmp.reserve(filenameFilters_pri.size());
                    for (const QString& filenameFilter_ite_con : filenameFilters_pri)
                    {
                        QRegExp rxTmp(filenameFilter_ite_con);
                        rxTmp.setPatternSyntax(QRegExp::Wildcard);
                        filtersTmp.emplace_back(rxTmp);
                    }
                }
                //for each file of the list
                for (fileStatus_s& remoteFileStatus_ite : remoteFileStatusArrayTmp.fileStatusVector_pub)
                {
                    remoteFileStatus_ite.iterated_pub = true;
                    auto remoteFindResultTmp(remoteFileStatusUMAP_pri.find(remoteFileStatus_ite.filename_pub.toStdString()));
                    if(remoteFindResultTmp != remoteFileStatusUMAP_pri.end())
                    {
                        remoteFindResultTmp->second.iterated_pub = true;
                        if (remoteFindResultTmp->second.hash_pub != remoteFileStatus_ite.hash_pub)
                        {
                            //remoteUMAPChangedTmp = true;
                            remoteFindResultTmp->second.hash_pub = remoteFileStatus_ite.hash_pub;
                            remoteFindResultTmp->second.fileSize_pub = remoteFileStatus_ite.fileSize_pub;
                            remoteFindResultTmp->second.fileLastModificationDatetime_pub = remoteFileStatus_ite.fileLastModificationDatetime_pub;
                        }
                        else
                        {
                            //it's the same, do nothing
                        }
                    }
                    else
                    {
                        bool passesAnyFilenameFilterTmp(false);
                        if (not filtersTmp.empty())
                        {
                            for (const QRegExp& filenameFilterRegExp_ite : filtersTmp)
                            {
                                passesAnyFilenameFilterTmp = filenameFilterRegExp_ite.exactMatch(remoteFileStatus_ite.filename_pub);
                                if (passesAnyFilenameFilterTmp)
                                {
                                    break;
                                }
                            }
                        }
                        else
                        {
                            passesAnyFilenameFilterTmp = true;
                        }
                        if (passesAnyFilenameFilterTmp)
                        {
                            if (remoteFileStatus_ite.filename_pub.startsWith(sourcePath_f()))
                            {
                                //remoteUMAPChangedTmp = true;
                                remoteFileStatusUMAP_pri.insert_or_assign(remoteFileStatus_ite.filename_pub.toStdString(), remoteFileStatus_ite);
                            }
                        }
                    }
                }
//                for (const auto& remoteItem_ite : remoteFileStatusUMAP_pri)
//                {
//                    if (not remoteItem_ite.second.iterated_pub)
//                    {
//                        remoteUMAPChangedTmp = true;
//                    }
//                }
            }
            //getAddMutex_f(QString::number(id_pri).toStdString() + "remote")->unlock();
//            if (remoteUMAPChangedTmp)
//            {
//                compareRequired_pri = true;
//            }
//            else
//            {
//                //it's set to false at the begining of requestFileList
//            }
            if (not initialRemoteScanSet_pri)
            {
                initialRemoteScanSet_pri = true;
            }
        }
        if (not remoteTriedOnce_pri)
        {
            remoteTriedOnce_pri = true;
        }
    });
    QObject::connect(fileListRequestThreadTmp, &QObject::destroyed, [this]{ remoteScanThreadExists_pri = false; });
    QObject::connect(fileListRequestThreadTmp, &QThread::finished, fileListRequestThreadTmp, &QThread::deleteLater);
    fileListRequestThreadTmp->start();
    remoteScanThreadExists_pri = true;
}

void mirrorConfigSourceDestinationMapping_c::compareLocalAndRemote_f()
{
    if (not eines::signal::isRunning_f())
    {
        return;
    }
    //if (initialLocalScanSet_pri and initialRemoteScanSet_pri and compareRequired_pri and currentDownloadCount_pri == 0)
    while (initialRemoteScanSet_pri and currentDownloadCount_pri == 0)
    {
        //compareRequired_pri = false;

        //QMutexLocker MlockerLocalTmp(getAddMutex_f(QString::number(id_pri).toStdString() + "local"));
        QMutexLocker MlockerRemoteTmp(getAddMutex_f(QString::number(id_pri).toStdString() + "remote"));
        //getAddMutex_f(QString::number(id_pri).toStdString() + "local")->lock();
        //getAddMutex_f(QString::number(id_pri).toStdString() + "remote")->lock();

        //preliminary check to see if the configured source matches entirely one file (one file mirror) or a substring (folder mirroring)
        uint_fast64_t remoteSingleFileHash(0);
        bool remoteSingleFileIterated(false);
        int_fast64_t remoteSingleFileSize(0);
        isSingleFileSet_pri = false;
//        if (not isSingleFileSet_pri)
//        {
        for (auto& remoteItem_ite : remoteFileStatusUMAP_pri)
        {
            if (sourcePath_pri == remoteItem_ite.second.filename_pub)
            {
                isSingleFileSet_pri = true;
                isSingleFile_pri = true;
                remoteSingleFileHash = remoteItem_ite.second.hash_pub;
                remoteSingleFileIterated = remoteItem_ite.second.iterated_pub;
                remoteSingleFileSize  = remoteItem_ite.second.fileSize_pub;
                break;
            }
            if (//remoteItem_ite.second.filename_pub.startsWith(sourcePath_pri + QDir::separator()) or
                remoteItem_ite.second.filename_pub.startsWith(sourcePath_pri))
            {
                isSingleFileSet_pri = true;
                isSingleFile_pri = false;
                break;
            }
        }


        //saves wich remote directories hold file X when includeDirectoriesWithFileX_pri is not empty
        QSet<QString> directoryContainsFileX_pri;
        if (not includeDirectoriesWithFileX_pri.isEmpty())
        {
            for (auto& remoteItem_ite : remoteFileStatusUMAP_pri)
            {
                QFileInfo fileTmp(remoteItem_ite.second.filename_pub);
                if (fileTmp.fileName() == includeDirectoriesWithFileX_pri)
                {
                    directoryContainsFileX_pri.insert(fileTmp.absolutePath());
                }
            }
        }

//        }
        //this can happen is the server doesn't provide what the client is searching or
        //something is configured wrong
        //skip it, it will be tried again
        if (not isSingleFileSet_pri)
        {

        }
        else
        {
            if (isSingleFile_pri)
            {
                QFileInfo localFileTmp(destinationPath_pri);
                if (remoteSingleFileIterated and localFileTmp.exists())
                {
                    auto localFindResultTmp(localFileStatusUMAP_pri.find(destinationPath_pri.toStdString()));
                    if (localFindResultTmp != localFileStatusUMAP_pri.end())
                    {
                        localFindResultTmp->second.iterated_pub = true;
                        if (downloadingQSet_pri.contains(destinationPath_pri))
                        {
                            break;
                        }
                        qint64 datetimeTmp(localFileTmp.lastModified().toMSecsSinceEpoch());
                        if (localFindResultTmp->second.fileLastModificationDatetime_pub != datetimeTmp)
                        {
                            localFindResultTmp->second.fileLastModificationDatetime_pub = datetimeTmp;
                            localFindResultTmp->second.hash_pub = getFileHash_f(destinationPath_pri);
                        }
                        else
                        {
                            //filedatetime didn't change, assume nothing has happened locally
                        }

                        //compare hashes
                        if (remoteSingleFileHash != localFindResultTmp->second.hash_pub)
                        {
                            QMutexLocker lockerTmp1(getAddMutex_f(QString::number(id_pri).toStdString() + "download"));
                            //download
                            downloadingQSet_pri.insert(destinationPath_pri);
                            filesToDownload_pri.emplace_back(sourcePath_pri, destinationPath_pri, remoteSingleFileSize);
                        }
                        else
                        {
                            //same Hash, files are equal, do nothing
                        }
                    }
                    else
                    {
                        uint_fast64_t hashTmp(getFileHash_f(destinationPath_pri));

                        if (remoteSingleFileHash != hashTmp)
                        {
                            //add/update the UMap
                            fileStatus_s fileStatusObj(destinationPath_pri, localFindResultTmp->second.hash_pub, localFileTmp.lastModified().toMSecsSinceEpoch(), localFileTmp.size());
                            fileStatusObj.iterated_pub = true;
                            localFileStatusUMAP_pri.emplace(destinationPath_pri.toStdString(), fileStatusObj);

                            QMutexLocker lockerTmp1(getAddMutex_f(QString::number(id_pri).toStdString() + "download"));
                            //download
                            downloadingQSet_pri.insert(destinationPath_pri);
                            filesToDownload_pri.emplace_back(sourcePath_pri, destinationPath_pri, remoteSingleFileSize);
                        }
                        else
                        {
                            //same Hash, files are equal, do nothing
                        }
                    }
                }
                else
                {
                    //if exists, remove it
                    if (localFileTmp.exists() and syncDeletions_pri)
                    {
                        //remove local file on the spot
                        if (not QFile::remove(destinationPath_pri))
                        {
                            //#ifdef DEBUGJOUVEN
                            QOUT_TS("Failed to remove (was deleted on the remote side) file " << destinationPath_pri << endl);
                            //#endif
                        }
                    }
                }
            }
            else
            //directory
            {
                for (auto& remoteItem_ite : remoteFileStatusUMAP_pri)
                {
                    if (//remoteItem_ite.second.filename_pub.startsWith(sourcePath_pri + QDir::separator()) or
                        remoteItem_ite.second.filename_pub.startsWith(sourcePath_pri))
                    {
                        if (not includeDirectoriesWithFileX_pri.isEmpty())
                        {
                            QFileInfo fileInfoTmp(remoteItem_ite.second.filename_pub);
                            QString fileInfoParentPathTmp(fileInfoTmp.absolutePath());
                            if (not directoryContainsFileX_pri.contains(fileInfoParentPathTmp))
                            {
                                //skip to next file
                                continue;
                            }
                            else
                            {
                                //keep going
                            }
                        }
                        else
                        {
                            //keep going
                        }

                        if (not includeSubdirectories_pri)
                        {
                            QFileInfo fileInfoTmp(remoteItem_ite.second.filename_pub);
                            QString fileInfoParentPathTmp(fileInfoTmp.absolutePath());
                            if (fileInfoParentPathTmp == sourcePath_pri or (fileInfoParentPathTmp  + "/") == sourcePath_pri)
                            {
                                //keep going
                            }
                            else
                            {
                                //skip to next file
                                continue;
                            }
                        }

                        QString finalDestinationTmp;
                        QString destinationRelativePath;

                        int_fast32_t sourceSubstringIndex(0);

                        if (sourcePath_pri.endsWith('/'))
                        {
                            sourceSubstringIndex = remoteItem_ite.second.filename_pub.size() - sourcePath_pri.size();
                        }
                        else
                        {
                            sourceSubstringIndex = remoteItem_ite.second.filename_pub.size() - (sourcePath_pri.size() + 1);
                        }

                        if (noSubdirectoriesInDestination_pri)
                        {
                            //only use the filename
                            QFileInfo fileInfoTmp(remoteItem_ite.second.filename_pub);
                            destinationRelativePath = fileInfoTmp.fileName();
                        }
                        else
                        {
                            destinationRelativePath = remoteItem_ite.second.filename_pub.right(sourceSubstringIndex);
                        }

                        if (destinationPath_pri.endsWith('/'))
                        {
                            finalDestinationTmp = destinationPath_pri + destinationRelativePath;
                        }
                        else
                        {
                            finalDestinationTmp = destinationPath_pri + '/' + destinationRelativePath;
                        }

#ifdef DEBUGJOUVEN
                        //QOUT_TS("(mirrorConfigSourceDestinationMapping_c::compareLocalAndRemote_f) finalDestinationTmp " << finalDestinationTmp << endl);
#endif
                        //try to compare to the local umap
                        auto localFindResultTmp(localFileStatusUMAP_pri.find(finalDestinationTmp.toStdString()));
                        //it's in the UMap
                        if (localFindResultTmp != localFileStatusUMAP_pri.end())
                        {
                            QFileInfo localFileTmp(finalDestinationTmp);
                            if (remoteItem_ite.second.iterated_pub and localFileTmp.exists())
                            {
                                localFindResultTmp->second.iterated_pub = true;
                                if (downloadingQSet_pri.contains(finalDestinationTmp))
                                {
                                    continue;
                                }

                                qint64 lastModificationDatetimeTmp(localFileTmp.lastModified().toMSecsSinceEpoch());
                                if (lastModificationDatetimeTmp != localFindResultTmp->second.fileLastModificationDatetime_pub)
                                {
                                    localFindResultTmp->second.fileLastModificationDatetime_pub = lastModificationDatetimeTmp;
                                    localFindResultTmp->second.hash_pub = getFileHash_f(destinationPath_pri);
                                }

                                //compare hashes and download
                                if (remoteItem_ite.second.hash_pub != localFindResultTmp->second.hash_pub)
                                {
                                    QMutexLocker lockerTmp1(getAddMutex_f(QString::number(id_pri).toStdString() + "download"));
                                    //download
                                    downloadingQSet_pri.insert(finalDestinationTmp);
                                    filesToDownload_pri.emplace_back(remoteItem_ite.second.filename_pub, finalDestinationTmp, remoteItem_ite.second.fileSize_pub);
                                }
                                else
                                {
                                    //same Hash, files are equal, do nothing
                                }
                            }
                            else
                            {
#ifdef DEBUGJOUVEN
                                //QOUT_TS("(mirrorConfigSourceDestinationMapping_c::compareLocalAndRemote_f) not iterated " << finalDestinationTmp << endl);
#endif
                                if (localFileTmp.exists() and syncDeletions_pri)
                                {
                                    //not iterated (remote umap) and exists locally
                                    //remove on the spot
                                    if (not QFile::remove(finalDestinationTmp))
                                    {
                                        //#ifdef DEBUGJOUVEN
                                        QOUT_TS("Failed to remove (was deleted on the remote side) file " << finalDestinationTmp << endl);
                                        //#endif
                                    }
                                }
                            }
                        }
                        else
                        //doesn't exist in the UMap
                        {
                            QFileInfo localFileTmp(finalDestinationTmp);
                            //iterated (remote umap)
                            if (remoteItem_ite.second.iterated_pub and localFileTmp.exists())
                            {
                                uint_fast64_t hashTmp(getFileHash_f(destinationPath_pri));

                                //compare hashes and download
                                if (remoteItem_ite.second.hash_pub != hashTmp)
                                {
                                    //add to the UMap
                                    fileStatus_s fileStatusObj(finalDestinationTmp, hashTmp, localFileTmp.lastModified().toMSecsSinceEpoch(), localFileTmp.size());
                                    fileStatusObj.iterated_pub = true;
                                    localFileStatusUMAP_pri.emplace(finalDestinationTmp.toStdString(), fileStatusObj);

                                    QMutexLocker lockerTmp1(getAddMutex_f(QString::number(id_pri).toStdString() + "local"));
                                    //download
                                    downloadingQSet_pri.insert(finalDestinationTmp);
                                    filesToDownload_pri.emplace_back(remoteItem_ite.second.filename_pub, finalDestinationTmp, remoteItem_ite.second.fileSize_pub);
                                }
                                else
                                {
                                    //same Hash, files are equal, do nothing
                                }
                            }
                            else
                            {
                                if (localFileTmp.exists() and syncDeletions_pri)
                                {
                                    //not iterated (remote umap) and exists locally
                                    //remove on the spot
                                    if (not QFile::remove(finalDestinationTmp))
                                    {
                                        //#ifdef DEBUGJOUVEN
                                        QOUT_TS("Failed to remove (was deleted on the remote side) file " << finalDestinationTmp << endl);
                                        //#endif
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        //no match
                    }
                }

                //(remote) remove non-existing keys
                std::vector<std::string> remoteFileKeysToRemove;
                for (const auto& remoteFileStatus_ite_con : remoteFileStatusUMAP_pri)
                {
                    if (not remoteFileStatus_ite_con.second.iterated_pub)
                    {
                        remoteFileKeysToRemove.emplace_back(remoteFileStatus_ite_con.first);
                    }
                }

                for (const auto& remoteFileKey_ite_con : remoteFileKeysToRemove)
                {
        #ifdef DEBUGJOUVEN
                    //QOUT_TS("(mirrorConfigSourceDestinationMapping_c::compareLocalAndRemote_f) erase from remoteUmap " << QString::fromStdString(remoteFileKey_ite_con) << endl);
        #endif
                    remoteFileStatusUMAP_pri.erase(remoteFileKey_ite_con);
                }

                //(local) remove non-existing keys
                std::vector<std::string> localFileKeysToRemove;
                for (auto& localFileStatus_ite : localFileStatusUMAP_pri)
                {
                    if (not localFileStatus_ite.second.iterated_pub)
                    {
                        localFileKeysToRemove.emplace_back(localFileStatus_ite.first);
                    }
                    else
                    {
                        localFileStatus_ite.second.iterated_pub = false;
                    }
                }

                for (const auto& localFileKey_ite_con : localFileKeysToRemove)
                {
                    localFileStatusUMAP_pri.erase(localFileKey_ite_con);
                }
            }
        }

        //getAddMutex_f(QString::number(id_pri).toStdString() + "local")->unlock();
        //getAddMutex_f(QString::number(id_pri).toStdString() + "remote")->unlock();
        break;
    }
}

void mirrorConfigSourceDestinationMapping_c::download_f()
{
    if (filesToDownload_pri.empty())
    {
        return;
    }

    while (
           eines::signal::isRunning_f()
           and currentDownloadCount_pri < maxDownloadCount_pri
           and mirrorConfig_ext.maxCurrentDownloadCount_f() < mirrorConfig_ext.maxDownloadCountGlobal_f()
           and not filesToDownload_pri.empty()
    )
    {
        currentDownloadCount_pri = currentDownloadCount_pri + 1;
        QMutexLocker lockerTmp1(getAddMutex_f(QString::number(id_pri).toStdString() + "download"));
        downloadInfo_s frontItem(filesToDownload_pri.front());

        downloadClient_c* downloadClientObj = new downloadClient_c(sourceAddress_pri, sourceDownloadPort_pri, frontItem, deleteThenCopy_pri, qApp);
        QObject::connect(downloadClientObj, &QTcpSocket::destroyed, [=]
        {
            currentDownloadCount_pri = currentDownloadCount_pri - 1;
            QMutexLocker lockerTmp1(getAddMutex_f(QString::number(id_pri).toStdString() + "download"));
            downloadingQSet_pri.remove(frontItem.destination_pub);
//            if (filesToDownload_pri.empty())
//            {
//                //so compareLocalAndRemote waits for the local scan of this mapping after a "download phase"
//                //because otherwise it will have the old hashes on the local side and it might try to download
//                //the same files again
//                initialLocalScanSet_pri = false;
//            }
        });
        QObject::connect(downloadClientObj, &QTcpSocket::disconnected, downloadClientObj, &QObject::deleteLater);
        filesToDownload_pri.erase(filesToDownload_pri.begin());
    }
}

void mirrorConfigSourceDestinationMapping_c::checkValid_f()
{
    bool isValidTmp(true);
    if (sourcePath_pri.isEmpty())
    {
        isValidTmp = false;
        appendError_f("Source path is empty");
    }

    if (destinationPath_pri.isEmpty())
    {
        isValidTmp = false;
        appendError_f("Destination path is empty");
    }

    if (sourceAddressStr_pri.isEmpty())
    {
        isValidTmp = false;
        appendError_f("Source address is empty");
    }

    if (sourceRequestPort_pri == 0)
    {
        isValidTmp = false;
        appendError_f("Source file request port is not assigned");
    }

    if (sourceDownloadPort_pri == 0)
    {
        isValidTmp = false;
        appendError_f("Source download port is not assigned");
    }

    if (not sourceAddress_pri.setAddress(sourceAddressStr_pri))
    {
        QHostInfo qHostInfoTmp(QHostInfo::fromName(sourceAddressStr_pri));
        if (qHostInfoTmp.addresses().isEmpty())
        {
            appendError_f("No valid source server address or failed to resolve it");
            isValidTmp = false;
        }
        else
        {
            sourceAddress_pri = qHostInfoTmp.addresses().first();
#ifdef DEBUGJOUVEN
            QOUT_TS("mirrorConfig_c::checkValid_f() sourceAddress_pri " << sourceAddress_pri.toString() << endl);
#endif
        }
    }

    if (not sourcePath_pri.isEmpty())
    {
        if (destinationPath_pri.endsWith('/') and not sourcePath_pri.endsWith('/'))
        {
            sourcePath_pri.append('/');
        }
        if (not destinationPath_pri.endsWith('/') and sourcePath_pri.endsWith('/'))
        {
            destinationPath_pri.append('/');
        }
    }

    if (localCheckIntervalMilliseconds_pri < 1)
    {
        appendError_f("Local check interval time is invalid " + localCheckIntervalMilliseconds_pri);
        isValidTmp = false;
    }
    if (remoteCheckIntervalMilliseconds_pri < 1)
    {
        appendError_f("Remote check interval time is invalid " + remoteCheckIntervalMilliseconds_pri);
        isValidTmp = false;
    }
    if (localCheckIntervalMilliseconds_pri > 0 and remoteCheckIntervalMilliseconds_pri > 0)
    {
        gcdWaitMilliseconds_pri = boost::math::gcd(localCheckIntervalMilliseconds_pri, remoteCheckIntervalMilliseconds_pri);
    }

    isValid_pri = isValidTmp;
}

void mirrorConfigSourceDestinationMapping_c::compareLocalAndRemoteThread_f()
{
    if (not compareLocalAndRemoteThreadExists_pri)
    {
        threadedFunction_c* compareLocalAndRemoteThread_pri = new threadedFunction_c(std::bind(&mirrorConfigSourceDestinationMapping_c::compareLocalAndRemote_f, this), qApp);
        QObject::connect(compareLocalAndRemoteThread_pri, &QThread::destroyed, [this]{ compareLocalAndRemoteThreadExists_pri = false; });
        QObject::connect(compareLocalAndRemoteThread_pri, &QThread::finished, compareLocalAndRemoteThread_pri, &QThread::deleteLater);
        compareLocalAndRemoteThread_pri->start();
        //QOUT_TS("operationMapping_c::compareNumberAAndNumberBThread_f() starting compareNumberAAndNumberBThread" << endl);
        compareLocalAndRemoteThreadExists_pri = true;
    }
}

///////////////////////////
//mirroConfig_c stuff
///////////////////////////

//void mirrorConfig_c::localScan_f()
//{
//    for (mirrorConfigSourceDestinationMapping_c& sourceDestinationMapping_ite : sourceDestinationMappings_pri)
//    {
//        sourceDestinationMapping_ite.localScan_f();
//    }
//}

quint16 mirrorConfig_c::updateServerPort_f() const
{
    return updateServerPort_pri;
}

uint_fast32_t mirrorConfig_c::maxDownloadCountGlobal_f() const
{
    return maxDownloadCountGlobal_pri;
}

//std::vector<mirrorConfigSourceDestinationMapping_c> mirrorConfig_c::sourceDestinationMappings_f() const
//{
//    return sourceDestinationMappings_pri;
//}

void mirrorConfig_c::checkValid_f()
{
    bool validResult(true);
    if (selfServerAddressStr_pri.isEmpty())
    {
        appendError_f("No ip interface set, selfServerAddress, in config file config.json");
        validResult = false;
    }

    if (not selfServerAddress_pri.setAddress(selfServerAddressStr_pri))
    {
        QHostInfo qHostInfoTmp(QHostInfo::fromName(selfServerAddressStr_pri));
        if (qHostInfoTmp.addresses().isEmpty())
        {
            appendError_f("No valid self server address or failed to resolve it");
            validResult = false;
        }
        else
        {
            selfServerAddress_pri = qHostInfoTmp.addresses().first();
        }
    }

    if (updateServerPort_pri == 0)
    {
        appendError_f("No port set for the update server, updateServerPort, in config file config.json");
        validResult = false;
    }

    if (sourceDestinationMappings_pri.empty())
    {
        appendError_f("Empty mappings section in config file config.json");
        validResult = false;
    }

    std::vector<int_fast64_t> validIntervals;
    for (mirrorConfigSourceDestinationMapping_c& sourceDestinationMapping_ite : sourceDestinationMappings_pri)
    {
        sourceDestinationMapping_ite.checkValid_f();
        appendError_f(sourceDestinationMapping_ite.getError_f());
        if (sourceDestinationMapping_ite.isValid_f())
        {
            validIntervals.emplace_back(sourceDestinationMapping_ite.gcdWaitMilliseconds_f());
        }
        else
        {
            validResult = false;
        }
    }

//    if (not getError_f().isEmpty())
//    {
//        QOUT_TS("operationsConfig_c::checkValid() getError_f()\n" + getError_f() << endl);
//    }

    if(validResult)
    {
        //the third field in std::acumulate is the initial/default value
        gcdWaitMillisecondsAll_pri = std::accumulate(validIntervals.cbegin(), validIntervals.cend(), validIntervals.front(), [](const int_fast64_t a, const int_fast64_t b)
        {
            return boost::math::gcd(a, b);
        });

        eines::removeIfPredicateTrue_f(sourceDestinationMappings_pri, [](const auto& item_par_con){return not item_par_con->isValid_f();});
    }

    valid_pri = validResult;
}

void mirrorConfig_c::printAllRemoteFileLists_f() const
{
    QOUT_TS("\nPrinting current remote file lists\n");
    for (const mirrorConfigSourceDestinationMapping_c& sourceDestinationMapping_ite_con : sourceDestinationMappings_pri)
    {
        sourceDestinationMapping_ite_con.printRemoteFileList_f();
    }
}

void mirrorConfig_c::read_f(const QJsonObject &json)
{
    selfServerAddressStr_pri = json["selfServerAddress"].toString();
    updateServerPort_pri = json["updateServerPort"].toInt();
    tryPrintRemoteFileListOnceAndQuit_pri = json["tryPrintRemoteFileListOnceAndQuit"].toBool(false);

    QJsonArray arrayTmp(json["sourceDestinationMappings"].toArray());
    sourceDestinationMappings_pri.reserve(arrayTmp.size());
    for (const auto& item_ite_con : arrayTmp)
    {
        QJsonObject mirrorConfigSourceDestinationJsonObject = item_ite_con.toObject();
        mirrorConfigSourceDestinationMapping_c mirrorConfigSourceDestinationMappingTmp;
        mirrorConfigSourceDestinationMappingTmp.read_f(mirrorConfigSourceDestinationJsonObject);
        sourceDestinationMappings_pri.emplace_back(mirrorConfigSourceDestinationMappingTmp);
    }
}
void mirrorConfig_c::write_f(QJsonObject &json) const
{
    QJsonArray sourceDestinationMappingArrayTmp;
    for (const auto& sourceDestinationMapping_ite_con : sourceDestinationMappings_pri)
    {
        QJsonObject jsonObjectTmp;
        sourceDestinationMapping_ite_con.write_f(jsonObjectTmp);
        sourceDestinationMappingArrayTmp.append(jsonObjectTmp);
    }

    json["sourceDestinationMappings"] = sourceDestinationMappingArrayTmp;
    json["selfServerAddress"] = selfServerAddressStr_pri;
    json["updateServerPort"] = updateServerPort_pri;
    json["tryPrintRemoteFileListOnceAndQuit"] = tryPrintRemoteFileListOnceAndQuit_pri;
}

void mirrorConfig_c::checkRemoteFiles_f()
{
    for (mirrorConfigSourceDestinationMapping_c& sourceDestinationMapping_ite : sourceDestinationMappings_pri)
    {
        sourceDestinationMapping_ite.checkRemoteFiles_f();
    }
}


void mirrorConfig_c::compareLocalAndRemote_f()
{
    for (mirrorConfigSourceDestinationMapping_c& sourceDestinationMapping_ite : sourceDestinationMappings_pri)
    {
        sourceDestinationMapping_ite.compareLocalAndRemoteThread_f();
    }
}

void mirrorConfig_c::initialSetup_f()
{
    //QOUT_TS("operationsConfig_c::initialSetup_f() BEGIN" << endl);
    QCoreApplication::setApplicationName("constantMirrorClientTcp");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser commandLineParser;
    commandLineParser.setApplicationDescription("constantMirrorClientTcp description");
    commandLineParser.addHelpOption();
    commandLineParser.addVersionOption();
    commandLineParser.addPositionalArgument("target", "Optional, config.json full path, by default it tries to read this file on the same path where the executable is");

    //Process the actual command line arguments given by the user
    commandLineParser.process(*qApp);

    QString errorStr;
    while (errorStr.isEmpty())
    {
        QString configFilePathStr;
        const QStringList parsedPositionalArgs(commandLineParser.positionalArguments());
        if (parsedPositionalArgs.size() > 0)
        {
            QString configjsonAlternativePathStr(parsedPositionalArgs.at(0));
            if (configjsonAlternativePathStr.isEmpty())
            {
                errorStr.append("Config.json path is empty");
                break;
            }

            if (not QFile::exists(configjsonAlternativePathStr))
            {
                errorStr.append("Config.json path doesn't exist " + configjsonAlternativePathStr);
                break;
            }
            configFilePathStr = configjsonAlternativePathStr;
        }

        if (configFilePathStr.isEmpty())
        {
            configFilePathStr = QCoreApplication::applicationDirPath() + "/config.json";
        }
        //qout_glo << "configFilePathStr " << configFilePathStr << endl;
        QFile configFile(configFilePathStr);
        if (not configFile.exists())
        {
            errorStr.append(
"Config file, config.json, doesn't exist.\nIt has to exist on the same path as the"
R"( constantMirrorClient executable or must be target/"first argument" when calling)"
R"( constantMirrorClient. It must have the following structure:)""\n"
R"({
    "selfServerAddress": "192.168.1.5"
    , "updateServerPort": "0"
    , "sourceDestinationMappings": [
        {
            "localCheckIntervalMilliseconds": 1000
            , "remoteCheckIntervalMilliseconds": 15000
            , "sourceAddress": "someIPOrDNS"
            , "sourceRequestPort": "30011"
            , "sourceDownloadPort": "30012"
            , "sourcePath": "/source/path"
            , "filenameFilters": [
                "*.exe",
                "*.bat"
            ],
            "includeSubdirectories": true,
            "includeDirectoriesWithFileX": true,
            "noSubdirectoriesInDestination": true,
            "destinationPath": "/destination/path",
            "syncDeletions": true,
            "deleteThenCopy": true
        }
    ]
}
*Everything is mandatory unless stated otherwise.

"selfServerAddress" ip/dns to use when setting a server

"updateServerPort" which port to use to setup a server where remote server/s will make a request when something has changed on their side

"sourceDestinationMappings[x].localCheckIntervalMilliseconds" optional, interval to check for changes locally. 1000 ms by default

"sourceDestinationMappings[x].remoteCheckIntervalMilliseconds" optional, interval to check for changes remotely. 15000 ms by default

"sourceDestinationMappings[x].sourceAddress" ip/dns to do file requests/downloads

"sourceDestinationMappings[x].sourceRequestPort" port to do file requests

"sourceDestinationMappings[x].sourceDownloadPort" to do file downloads

"sourceDestinationMappings[x].sourcePath" remote file or directory path to fetch files from

"sourceDestinationMappings[x].destinationPath" optional, local file or directory path to save the fetched files, if not specified sourcePath is used as destination

"sourceDestinationMappings[x].filenameFilters" optional, when dealing with a directory source (not for single files) filter filenames, all by default

"sourceDestinationMappings[x].includeSubdirectories" optional, when dealing with a remote directory include/recurse subdirectories for files to fetch, true by default

"sourceDestinationMappings[x].includeDirectoriesWithFileX" optional, for directory mirroring, only mirror the contents of remote diretories that contain a particular filename

"sourceDestinationMappings[x].noSubdirectoriesInDestination" optional, when dealing with a remote directory put all the files on the root of the destination ignoring any subdirectory structure from the remote side, this option can be dangerous if there a files with the same name in different directories

"sourceDestinationMappings[x].syncDeletions" optional, sync remote deletions that happen while this process is running, on the destination, replacing will always happen despite this option, true by default

"sourceDestinationMappings[x].deleteThenCopy" optional, if true delete the original and then download, if false rename original then download and finally delete original, false by default)");
                    break;
        }

        QByteArray jsonByteArray;
        if (configFile.open(QIODevice::ReadOnly))
        {
            jsonByteArray = configFile.readAll();
        }
        else
        {
            errorStr.append("Could not open config file config.json");
            break;
        }

        auto jsonDocObj(QJsonDocument::fromJson(jsonByteArray));
        if (jsonDocObj.isNull())
        {
            errorStr.append("Could not parse json from the config file config.json");
            //qout << "jsonByteArray " << jsonByteArray << endl;
            //qout << "isNull jsonDocObj" << endl;
            break;
        }
        else
        {
            read_f(jsonDocObj.object());
        }

        checkValid_f();
        errorStr.append(getError_f());

        break;
    }
    if (not errorStr.isEmpty())
    {
        QOUT_TS("Errors:\n" << errorStr << endl);
        returnValue_ext = EXIT_FAILURE;
        eines::signal::stopRunning_f();
        return;
    }
    else
    {
        if (valid_pri)
        {
#ifdef DEBUGJOUVEN
            QOUT_TS("operationsConfig_c::initialSetup_f() valid " << endl);
#endif
            QSslConfiguration sslOptions(QSslConfiguration::defaultConfiguration());
            sslOptions.setSslOption(QSsl::SslOptionDisableCompression, false);
            sslOptions.setPeerVerifyMode(QSslSocket::VerifyNone);
            eines::sslUtils_c sslUtilsTmp;
            sslOptions.setPrivateKey(QSslKey(QByteArray::fromStdString(sslUtilsTmp.privateKeyStr_f()), QSsl::Rsa));
            sslOptions.setLocalCertificate(QSslCertificate(QByteArray::fromStdString(sslUtilsTmp.keyCertificateStr_f())));

            QSslConfiguration::setDefaultConfiguration(sslOptions);

            //start the update server
            new updateServer_c(
                        selfServerAddress_pri
                        , updateServerPort_pri
                        , qApp
            );
            //no need it will be destroyed with the qcoreapplication instance when the program ends
            //QObject::connect(updateServerObj, &QTcpServer::destroyed, updateServerObj, &QObject::deleteLater);

            int_fast64_t tmpCycleTimeoutMilliseconds((gcdWaitMillisecondsAll_pri / 4) - 1);
            if (tmpCycleTimeoutMilliseconds < 1)
            {
                tmpCycleTimeoutMilliseconds = 1;
            }
            //else if the machine goes down it might not wait for the program to finish
            if (tmpCycleTimeoutMilliseconds > 5000)
            {
                tmpCycleTimeoutMilliseconds = 5000;
            }

            mainLoopTimer_pri = new QTimer(qApp);
            QObject::connect(mainLoopTimer_pri, &QTimer::timeout, std::bind(&mirrorConfig_c::mainLoop_f, &mirrorConfig_ext));
#ifdef DEBUGJOUVEN
            QOUT_TS("operationsConfig_c::initialSetup_f() tmpCycleTimeout " << tmpCycleTimeoutMilliseconds << endl);
#endif
            mainLoopTimer_pri->start(tmpCycleTimeoutMilliseconds);
        }
    }
}

void mirrorConfig_c::downloadFiles_f()
{
    for (mirrorConfigSourceDestinationMapping_c& sourceDestinationMapping_ite : sourceDestinationMappings_pri)
    {
        sourceDestinationMapping_ite.download_f();
    }
}

uint_fast32_t mirrorConfig_c::maxCurrentDownloadCount_f() const
{
    uint_fast32_t counter(0);
    for (const mirrorConfigSourceDestinationMapping_c& item_ite_con : sourceDestinationMappings_pri)
    {
        counter = counter + item_ite_con.currentDownloadCount_f();
    }
    return counter;
}

bool mirrorConfig_c::allRemotesTriedOnce_f() const
{
    for (const mirrorConfigSourceDestinationMapping_c& item_ite_con : sourceDestinationMappings_pri)
    {
        if (not item_ite_con.remoteTriedOnce_f())
        {
            return false;
        }
    }
    return true;
}

void mirrorConfig_c::setRemoteHasUpdated_f(
        const QHostAddress &address_par_con
        , const quint16 port_par_con)
{
    //check if any mapping uses this port+address and "force" it to request the file list again
    for (mirrorConfigSourceDestinationMapping_c& item_ite : sourceDestinationMappings_pri)
    {
        //it's easier to check the port first
        //check if the "port" sent/requested by the server matches
        if (port_par_con == item_ite.sourceRequestPort_f())
        {
#ifdef DEBUGJOUVEN
            //QOUT_TS("(mirrorConfig_c::setRemoteHasUpdated_f) item_ite.sourceRequestPort_f() " << item_ite.sourceRequestPort_f() << endl);
#endif
            //the mapped address is resolved at the start of the program
            //and it must BE SOMETHING otherwise the program wouldn't make it this far
#ifdef DEBUGJOUVEN
            //QOUT_TS("(mirrorConfig_c::setRemoteHasUpdated_f) item_ite.sourceAddress_f() " << item_ite.sourceAddress_f().toString() << endl);
#endif
            if (address_par_con == item_ite.sourceAddress_f())
            {
#ifdef DEBUGJOUVEN
                //QOUT_TS("(mirrorConfig_c::setRemoteHasUpdated_f) item_ite.setRemoteHasUpdated_f();" << endl);
#endif
                item_ite.setRemoteHasUpdated_f();
                continue;
            }
            else
            {
                //addresses don't match
            }
        }
        else
        {
            //ports don't match
        }
    }
}

void mirrorConfig_c::mainLoop_f()
{
    //QOUT_TS("mirrorConfig_c::mainLoop() start" << endl);
    //QOUT_TS("operationsConfig_c::mainLoop() qthreads counter " << QThreadCount_f() << endl);
    if (eines::signal::isRunning_f())
    {
        if (not tryPrintRemoteFileListOnceAndQuit_pri)
        {
            //not threaded, but it's pure network+IO so it's async event driven
            downloadFiles_f();

            //threaded (1 thread per mapping)
            compareLocalAndRemote_f();

//            if (not localScanThreadExists_pri)
//            {
//                //1 thread to do the local scan for all the mappings
//                threadedFunction_c* localScanThreadTmp = new threadedFunction_c(std::bind(&mirrorConfig_c::localScan_f, this), qApp);
//                QObject::connect(localScanThreadTmp, &QThread::destroyed, [this]
//                {
//                    localScanThreadExists_pri = false;
//                    //QOUT_TS("localScanThreadExists_pri = false" << endl);
//                });
//                QObject::connect(localScanThreadTmp, &QThread::finished, localScanThreadTmp, &QThread::deleteLater);
//                localScanThreadTmp->start();
//                //QOUT_TS("operationsConfig_c::mainLoop() starting calculateNumberAsThread" << endl);
//                localScanThreadExists_pri = true;
//            }
        }
        else
        {
            if (allRemotesTriedOnce_f())
            {
                //print files per mapping/server and then quit
                printAllRemoteFileLists_f();
                eines::signal::stopRunning_f();
            }
        }

        //threaded (1 thread per mapping)
        checkRemoteFiles_f();
    }
    else
    {
        if (eines::signal::threadCount_f() > 1 or qThreadCount_f() > 0)
        {
//            QOUT_TS("qthreads counter " << QThreadCount_f() << endl);
//            QOUT_TS("eines::signal::threadCount_f() " << eines::signal::threadCount_f() << endl);
            //change the interval and wait another cycle
            if (mainLoopTimer_pri->interval() != 10)
            {
                mainLoopTimer_pri->start(10);
            }
        }
        else
        {
#ifdef DEBUGJOUVEN
            QOUT_TS("qthreads counter " << qThreadCount_f() << endl);
            QOUT_TS("eines::signal::threadCount_f() " << eines::signal::threadCount_f() << endl);
            QOUT_TS("QCoreApplication::exit();"<< endl);
#endif
            QCoreApplication::exit();
        }
    }

    //QOUT_TS("mirrorConfig_c::mainLoop() end" << endl);
}

mirrorConfig_c mirrorConfig_ext;

fileStatusArrayPlusHostInfo_s::fileStatusArrayPlusHostInfo_s(
        const std::unordered_map<std::string, fileStatus_s> &fileStatusUMAP_par_con,
        const QString &hostStr_par_con
        , const quint16 port_par_con)
    :
    fileStatusArray_s(fileStatusUMAP_par_con)
    ,  hostStr_pub(hostStr_par_con)
  , port_pub(port_par_con)
{}

void fileStatusArrayPlusHostInfo_s::read_f(const QJsonObject &json)
{
    fileStatusArray_s::read_f(json);
    port_pub = json["port"].toInt();
    hostStr_pub = json["host"].toString();
}

void fileStatusArrayPlusHostInfo_s::write_f(QJsonObject &json) const
{
    fileStatusArray_s::write_f(json);
    json["port"] = port_pub;
    json["host"] = hostStr_pub;
}

