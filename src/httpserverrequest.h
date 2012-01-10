/*  This file is part of the Tufão project
    Copyright (C) 2011 Vinícius dos Santos Oliveira <vini.ipsmaker@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any
    later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HTTPSERVERREQUEST_H
#define HTTPSERVERREQUEST_H

#include "httpserverresponse.h"

class QAbstractSocket;

namespace Tufao {

struct Headers;

namespace Priv {

struct HttpServerRequest;

} // namespace Priv

/*!
  \brief The Tufao::HttpServer represents a HTTP request received by
  Tufao::HttpServer.

  \note
  You can use it to create your own HTTP servers too, just handle the
  connections steps and pass the connection objects to Tufao::HttpServerRequest
  constructor.
  This may be useful if you want to create a threaded web server or use non
  conventional streams instead tcp sockets.

  \sa
  Tufao::HttpServer
  */
class TUFAO_EXPORT HttpServerRequest : public QObject
{
    Q_OBJECT
public:
    enum HttpVersion
    {
        HTTP_1_0,
        HTTP_1_1
    };

    /*!
      Constructs a Tufao::HttpServerRequest object.

      \p parent is passed to the QObject constructor.

      \param socket The connection used by Tufao::HttpServerRequest to receive
      HTTP messages. If you pass NULL, the object will be useless.
      */
    explicit HttpServerRequest(QAbstractSocket *socket, QObject *parent = 0);

    /*!
      Destroys the object.
      */
    ~HttpServerRequest();

    /*!
      The request method.

      It can assume the following values:
        - "HEAD"
        - "GET"
        - "POST"
        - "PUT"
        - "DELETE"
        - "TRACE"
        - "OPTIONS"
        - "CONNECT"
        - "PATCH"
        - "COPY"
        - "LOCK"
        - "MKCOl"
        - "MOVE"
        - "PROPFIND"
        - "PROPPATCH"
        - "UNLOCK"
        - "REPORT"
        - "MKACTIVITY"
        - "CHECKOUT"
        - "MERGE"
        - "MSEARCH"
        - "NOTIFY"
        - "SUBSCRIBE"
        - "UNSUBSCRIBE"
      */
    QByteArray method() const;

    /*!
      The request URL.

      This contains only the URL that is present in the actual HTTP request.
      If the request line is:

      \verbatim
GET /login?username=tux HTTP/1.1\r\n
Accept: text/plain\r\n
\r\n
      \endverbatim

      Then Tufao::HttpServerRequest::url() will be "/login?username=tux"

      \sa
      Tufao::Url
      */
    QByteArray url() const;

    /*!
      The HTTP headers sent by the client. These headers are fully populated
      when the signal Tufao::HttpServerRequest:ready signal is emitted.

      \sa
      Tufao::HttpServerRequest::trailers()
      */
    Headers headers() const;

    /*!
      The HTTP trailers (if present). Only populated after the
      Tufao::HttpServerRequest::end signal.
      */
    Headers trailers() const;

    /*!
      Returns the HTTP protocol version used in the request.
      */
    HttpVersion httpVersion() const;

    /*!
      The QAbstractSocket object associated with the connection.

      This will be a QTcpSocket object if created by Tufao::HttpServer and a
      QSslSocket if created by Tufao::HttpsServer.
      */
    QAbstractSocket *socket() const;

signals:
    /*!
      This signal is emitted when most of the data about the request was
      gathered.

      After this signal is emitted, you can safely interpret the request and the
      only missing part may be (if any) the message body.

      \sa
      Tufao::HttpServerRequest::data
      Tufao::HttpServerRequest::end
      */
    void ready(Tufao::HttpServerResponse::Options);

    /*!
      This signal is emitted each time a piece of the message body is received.
      */
    void data(QByteArray data);

    /*!
      This signal is emitted exactly once for each request.

      After that, no more data signals will be emitted for this session. A new
      session (if any) will be only initiated after you respond the request.
      */
    void end();

    /*!
      This signal is emitted when the underlaying connection is closed (also
      caused by invalid requests).

      Just like Tufao::HttpServerRequest::end, this signal is emitted only once
      per request, and no more data signals will fire afterwards.
      */
    void close();

    /*!
      This signal is emitted when a http upgrade is requested.

      \param head The initial bytes from the new connection protocol.
      */
    void upgrade(QByteArray head);

private slots:
    void onReadyRead();

private:
    void clearBuffer();
    void clearRequest();

    Priv::HttpServerRequest *priv;

    friend struct Tufao::Priv::HttpServerRequest;
};

} // namespace Tufao

#endif // HTTPSERVERREQUEST_H