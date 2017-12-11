# constantMirrorClientAsync
Client-side program that mirrors remote paths to the local filesystem using tcp sockets. 

A V2 of https://github.com/jouven/constantMirrorClient, the reasoning is that recently I tried to rsync a folder with files, 5xx GB, using the checksum flag "-c", rsync, like my first version of this program, first must generate ALL the hashes before starting any data transmission. 

The server first does a scan so it can list all the files at the start, initially the clients can only check the size. Then starts hashing everything and the file request updates accordingly so enventually the clients can check precisely if it's the same file.

The client can download each file if it detects it's different, first it checks size and if it's the same size it hashes the file and then checks the hash. This way a client can start downloading before waiting for the server, or itself, to hash everything first, the file hash will be checked eventually when the file size matches and both the server and the client have hashed the file.



Works with https://github.com/jouven/constantMirrorServerAsync

Compilation
-----------
Requires:

Qt library

https://github.com/jouven/signalso

https://github.com/jouven/fileHashQtso

https://github.com/jouven/baseClassQtso

https://github.com/jouven/essentialQtso

https://github.com/jouven/threadedFunctionQtso

https://github.com/jouven/qmutexUMapQtso

https://github.com/jouven/sslUtilsso

Openssl

Probably a header from https://github.com/jouven/comuso

Run (in constantMirrorClientAsync source directory or pointing to it):

    qmake

and then:

    make
