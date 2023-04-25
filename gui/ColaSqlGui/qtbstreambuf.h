#ifndef QTBSTREAMBUF_H
#define QTBSTREAMBUF_H

#include <QTextBrowser>
#include <QTextEdit>
#include <streambuf>

class QTextEditStreamBuf : public std::streambuf {
 public:
  QTextEditStreamBuf(QTextEdit *edit) : m_Edit(edit) {}

 protected:
  virtual std::streamsize xsputn(const char *s, std::streamsize n) override {
    QString text = QString::fromUtf8(s, static_cast<int>(n));
    m_Edit->append(text);
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
  QTextEdit *m_Edit;
};

#endif  // QTBSTREAMBUF_H
