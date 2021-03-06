/* This file is part of QTextPad.
 *
 * QTextPad is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QTextPad is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QTextPad.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "charsets.h"

#include <QTextCodec>
#include <QCoreApplication>
#include <QLoggingCategory>
#include <map>

Q_LOGGING_CATEGORY(CsLog, "qtextpad.charsets", QtInfoMsg)

// Greatly simplified version from KCharsets with no Latin-1 fallback
QTextCodec *QTextPadCharsets::codecForName(const QString &name)
{
    // KCharsets handles this one specially...  Not sure why
    if (name == QLatin1String("gb2312") || name == QLatin1String("gbk"))
        return QTextCodec::codecForName("gb18030");

    QByteArray nameLatin1 = name.toLatin1();
    auto codec = QTextCodec::codecForName(nameLatin1);
    if (codec)
        return codec;

    nameLatin1 = nameLatin1.toLower();
    if (nameLatin1.endsWith("_charset"))
        nameLatin1.chop(8);
    if (nameLatin1.startsWith("x-"))
        nameLatin1.remove(0, 2);

    if (name.isEmpty())
        return Q_NULLPTR;

    return QTextCodec::codecForName(nameLatin1);
}

// Data originally from KCharsets with a few additions.  However, KCharsets is
// arguably worse than pure QTextCodec since it is wildly inconsistent in the
// formats it uses for encoding names, and still manages to include several
// very confusingly named duplicates (see UTF-16, ucs2, ISO 10646-UCS-2
// for example, all of which are the same ambiguous UTF-16 codec)
QTextPadCharsets::QTextPadCharsets()
{
    (void)new Cp437Codec;

    m_encodingCache.reserve(17);
    m_encodingCache.append(QStringList {
        tr("Arabic"),
        "ISO-8859-6",
        "windows-1256",
    });
    m_encodingCache.append(QStringList {
        tr("Baltic"),
        "ISO-8859-4",
        "ISO-8859-13",
        "windows-1257",
    });
    m_encodingCache.append(QStringList {
        tr("Central European"),
        "ISO-8859-2",
        "ISO-8859-3",
        "ISO-8859-10",
        "windows-1250",
    });
    m_encodingCache.append(QStringList {
        tr("Chinese Simplified"),
        "GB18030",
        "GBK",
    });
    m_encodingCache.append(QStringList {
        tr("Chinese Traditional"),
        "Big5",
        "Big5-HKSCS",
    });
    m_encodingCache.append(QStringList {
        tr("Cyrillic"),
        "IBM866",
        "ISO-8859-5",
        "KOI8-R",
        "KOI8-U",
        "windows-1251",
    });
    m_encodingCache.append(QStringList {
        tr("Eastern European"),
        "ISO-8859-16",
    });
    m_encodingCache.append(QStringList {
        tr("Greek"),
        "ISO-8859-7",
        "windows-1253",
    });
    m_encodingCache.append(QStringList {
        tr("Hebrew"),
        "ISO-8859-8",
        "windows-1255",
    });
    m_encodingCache.append(QStringList {
        tr("Indic"),
        "iscii-bng",
        "iscii-dev",
        "iscii-gjr",
        "iscii-knd",
        "iscii-mlm",
        "iscii-ori",
        "iscii-pnj",
        "iscii-tlg",
        "iscii-tml",
    });
    m_encodingCache.append(QStringList {
        tr("Japanese"),
        "EUC-JP",
        "ISO-2022-JP",
        "Shift-JIS",
    });
    m_encodingCache.append(QStringList {
        tr("Korean"),
        "EUC-KR",
        "windows-949",
    });
    m_encodingCache.append(QStringList {
        tr("Thai"),
        "IBM874",
        "TIS-620",
    });
    m_encodingCache.append(QStringList {
        tr("Turkish"),
        "ISO-8859-9",
        "windows-1254",
    });
    m_encodingCache.append(QStringList {
        tr("Western European"),
        "IBM850",
        "ISO-8859-1",
        "ISO-8859-14",
        "ISO-8859-15",
        "hp-roman8",
        "windows-1252",
    });
    m_encodingCache.append(QStringList {
        tr("Other"),
        "macintosh",
        "OEM437",
        "TSCII",
        "windows-1258",
    });
    m_encodingCache.append(QStringList {
        tr("Unicode"),
        "UTF-7",
        "UTF-8",
        "UTF-16LE",
        "UTF-16BE",
        "UTF-32LE",
        "UTF-32BE",
    });

    std::map<QTextCodec *, QStringList> codecDupes;

    // Prune encodings that aren't supported by Qt or the platform
    for (auto &encodingList : m_encodingCache) {
        int enc = 1;
        while (enc < encodingList.size()) {
            const QString &name = encodingList.at(enc);
            QTextCodec *codec = QTextCodec::codecForName(name.toLatin1());
            if (!codec) {
                qCDebug(CsLog, "Removing unsupported codec %s", qPrintable(name));
                encodingList.removeAt(enc);
            } else {
                codecDupes[codec].append(name);
                ++enc;
            }
        }
    }
    int script = 0;
    while (script < m_encodingCache.size()) {
        // One entry means only the script name is present with no encodings
        if (m_encodingCache.at(script).size() == 1)
            m_encodingCache.removeAt(script);
        else
            ++script;
    }

    for (const auto &dupe : codecDupes) {
        if (dupe.second.size() > 1) {
            qCDebug(CsLog, "Duplicate codecs for %s:", qPrintable(dupe.first->name()));
            for (const auto &name : dupe.second)
                qCDebug(CsLog, "  * %s", qPrintable(name));
        }
    }
}

QTextPadCharsets *QTextPadCharsets::instance()
{
    static QTextPadCharsets s_charsets;
    return &s_charsets;
}

QList<QStringList> QTextPadCharsets::encodingsByScript()
{
    return instance()->m_encodingCache;
}
