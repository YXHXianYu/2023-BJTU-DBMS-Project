#ifndef QTBSTREAMBUF_H
#define QTBSTREAMBUF_H

#include <QTextBrowser>
#include <streambuf>

class QTextBrowserStreamBuf : public std::streambuf {
 public:
  QTextBrowserStreamBuf(QTextBrowser *browser) : m_browser(browser) {}

 protected:
  virtual std::streamsize xsputn(const char *s, std::streamsize n) override {
    QString text = QString::fromUtf8(s, static_cast<int>(n));
    m_browser->append(text);
    return n;
  }

  virtual int_type overflow(int_type v) override {
    if (v != traits_type::eof()) {
      char ch = static_cast<char>(v);
      xsputn(&ch, 1);
    }
    return v;
  }

 private:
  QTextBrowser *m_browser;
};

#endif  // QTBSTREAMBUF_H
