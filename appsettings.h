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

#ifndef _APPSETTINGS_H
#define _APPSETTINGS_H

#include <QSettings>

#define SIMPLE_SETTING(type, name, get, set, defaultValue) \
    type get() const { return m_settings.value(name, defaultValue).value<type>(); } \
    void set(type value) { m_settings.setValue(name, value); }

struct RecentFile
{
    QString m_path;
    QString m_encoding;
    unsigned int m_line;
};

Q_DECLARE_METATYPE(RecentFile)

class QTextPadSettings
{
public:
    QTextPadSettings();
    QString settingsDir() const;

    QList<RecentFile> recentFiles() const;
    void addRecentFile(const QString &filename, const QString &encoding,
                       unsigned int line);
    void clearRecentFiles();

    unsigned int recentFilePosition(const QString &filename);
    void setRecentFilePosition(const QString &filename, unsigned int line);

    SIMPLE_SETTING(bool, "ShowToolBar", showToolBar, setShowToolBar, true)
    SIMPLE_SETTING(bool, "ShowStatusBar", showStatusBar, setShowStatusBar, true)
    SIMPLE_SETTING(bool, "ShowFilePath", showFilePath, setShowFilePath, false)

    // Editor settings
    SIMPLE_SETTING(bool, "Editor/WordWrap", wordWrap, setWordWrap, false)
    SIMPLE_SETTING(bool, "Editor/ShowLongLineMargin", showLongLineMargin,
                   setShowLongLineMargin, false)
    SIMPLE_SETTING(int, "Editor/LongLineWidth", longLineWidth, setLongLineWidth, 80)
    SIMPLE_SETTING(bool, "Editor/IndentationGuides", indentationGuides,
                   setIndentationGuides, false)
    SIMPLE_SETTING(bool, "Editor/LineNumbers", lineNumbers, setLineNumbers, false)
    SIMPLE_SETTING(bool, "Editor/ShowWhitespace", showWhitespace, setShowWhitespace, false)
    SIMPLE_SETTING(bool, "Editor/HighlightCurrentLine", highlightCurLine,
                   setHighlightCurLine, true)
    SIMPLE_SETTING(bool, "Editor/MatchBraces", matchBraces, setMatchBraces, true)

    SIMPLE_SETTING(int, "Editor/TabWidth", tabWidth, setTabWidth, 4)
    SIMPLE_SETTING(int, "Editor/IndentWidth", indentWidth, setIndentWidth, 4)
    // -1 or any other invalid value will trigger the setting of the default
    SIMPLE_SETTING(int, "Editor/IndentMode", indentMode, setIndentMode, -1)
    SIMPLE_SETTING(bool, "Editor/AutoIndent", autoIndent, setAutoIndent, true)

    SIMPLE_SETTING(bool, "Editor/ScrollPastEndOfFile", scrollPastEndOfFile,
                   setScrollPastEndOfFile, false)

    QFont editorFont() const;
    void setEditorFont(const QFont &font);

    QString editorTheme() const { return m_settings.value("Editor/Theme").toString(); }
    void setEditorTheme(const QString &theme) { m_settings.setValue("Editor/Theme", theme); }

    QSize windowSize() const;
    void setWindowSize(const QSize &size) { m_settings.setValue("WindowSize", size); }

    // Search dialog options
    QStringList recentSearches() const;
    void addRecentSearch(const QString &text);

    QStringList recentSearchReplacements() const;
    void addRecentSearchReplacement(const QString &text);

    SIMPLE_SETTING(bool, "Search/CaseSensitive", searchCaseSensitive,
                   setSearchCaseSensitive, false)
    SIMPLE_SETTING(bool, "Search/WholeWord", searchWholeWord,
                   setSearchWholeWord, false)
    SIMPLE_SETTING(bool, "Search/Regex", searchRegex, setSearchRegex, false)
    SIMPLE_SETTING(bool, "Search/Escapes", searchEscapes, setSearchEscapes, false)
    SIMPLE_SETTING(bool, "Search/Wrap", searchWrap, setSearchWrap, true)

private:
    QSettings m_settings;
};

/* Helper for loading theme icons */
#define ICON(name)  QIcon::fromTheme(QStringLiteral(name), \
                                     QIcon(QStringLiteral(":/icons/" name ".png")))

#endif // _APPSETTINGS_H
