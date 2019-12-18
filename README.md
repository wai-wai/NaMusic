# NaMusic
# 开发环境
#### win10 系统
    Qt5.8以上版本
    ssl版本通过qDebug()<<QSslSocket::sslLibraryBuildVersionString();
    然后再ssl官网下载对应的安装包，安装后 从bin目录复制
    libssl-1_1.dll， libcrypto-1_1.dll 到Qt对应的:\Qt\Qt5.14.0\5.14.0\mingw73_32\bin\下即可
# 增加试听
    试听默认下载最小音质 测试ok
    修改界面缩放问题
    增加试听下载 默认目录QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/music/";
