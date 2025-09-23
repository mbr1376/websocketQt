#include "decrypthelper.h"

DecryptHelper::DecryptHelper() :
    key("1234567890abcdef"),
    iv("fedcba0987654321"),
    aes(QAESEncryption::AES_128, QAESEncryption::CBC)
{}

QString DecryptHelper::decrypt(const QString &encryptedBase64)
{
    QByteArray encrypted = QByteArray::fromBase64(encryptedBase64.toUtf8());
    QByteArray decrypted = aes.decode(encrypted, key, iv);
    decrypted = QAESEncryption::RemovePadding(decrypted);
    return QString::fromUtf8(decrypted);
}
