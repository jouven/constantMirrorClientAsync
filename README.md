# constantMirrorClientAsync
Client-side program that mirrors remote paths to the local filesystem using tcp sockets. 

A V2 of https://github.com/jouven/constantMirrorClient, the reasoning is that recently I tried to rsync a folder with files, 5xx GB, using the checksum flag "-c", rsync, like my first version of this program, first must generate ALL the hashes before starting any data transmission. 

This version allows to download each file after it has been hashed so a client can start downloading before waiting for the server to hash everything first.

Works with https://github.com/jouven/constantMirrorClientAsync

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
