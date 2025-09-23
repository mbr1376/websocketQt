#include "encrypthelper.h"

EncryptHelper::EncryptHelper():
    key("1234567890abcdef"),
    iv("fedcba0987654321"),
    aes(QAESEncryption::AES_128, QAESEncryption::CBC)
{

}

QString EncryptHelper::encrypt(const QString &strValue)
{
    QByteArray encrypted = aes.encode(strValue.toUtf8(), key, iv);
    return QString::fromUtf8(encrypted.toBase64());
}
