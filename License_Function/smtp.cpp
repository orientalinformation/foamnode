#include "smtp.h"

Smtp::Smtp( const QString &server, const QString &from, const QString &to, const QList<QString> &bcc, const QString &subject, const QString &body, const bool html ) 
{
    socket = new QTcpSocket( this );

    connect( socket, SIGNAL( readyRead() ), this, SLOT( readyRead() ) );
    connect( socket, SIGNAL( connected() ), this, SLOT( connected() ) );
    connect( socket, SIGNAL(error(QAbstractSocket::SocketError)), this, 
		SLOT(errorReceived(QAbstractSocket::SocketError)));   
    connect( socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, 
		SLOT(stateChanged(QAbstractSocket::SocketState)));       
    connect(socket, SIGNAL(disconnected()), this, 
		SLOT(disconnected()));; 

        //Generate date
	QTime myTime = QDateTime::currentDateTime().toUTC().time();
	QLocale myLocal = QLocale(QLocale::C);
	QString date = myLocal.toString(QDate::currentDate(), "ddd, dd MMM yyyy ");
	date += myLocal.toString(myTime, "hh:mm:ss");
	date += " +0000 (UTC)";
	
    message = tr("To: %1\n").arg(to);
    message.append(tr("From: %1\n").arg(from));
    message.append(tr("Subject: %1\n").arg(subject));
    message.append(tr("Date: %1\n").arg(date));
	
	if(html)
	{
        message.append(tr("MIME-Version: 1.0 \n"));
        message.append(tr("Content-Type: text/html; charset=\"iso-8859-1\"; format=flowed \n"));
	}
 	message.append(body);
	message.replace( QString::fromLatin1( "\n" ), QString::fromLatin1( "\r\n" ) );
	message.replace( QString::fromLatin1( "\r\n.\r\n" ), 
	QString::fromLatin1( "\r\n..\r\n" ) );
    this->from = from;
    
    //Add to adress
    //
    rcpt = bcc;
    rcpt.insert(0, to); 
    rcptIndex = 0;
    
    this->server = server;

}

void Smtp::send()
{
    state = Init;
    emit status( 1 ); //Connecting
    socket->connectToHost( server, 25);

    if(socket->waitForConnected ( 100000 )) 	{qDebug("constructor"); 	}

    t = new QTextStream( socket );
    emit status( 2 ); //Connected
}
Smtp::~Smtp()
{
    delete t;
    delete socket;
}
void Smtp::stateChanged(QAbstractSocket::SocketState socketState)
{

    qDebug() <<"stateChanged " << socketState;
}

void Smtp::errorReceived(QAbstractSocket::SocketError socketError)
{
    qDebug() << "error " <<socketError;
}

void Smtp::disconnected()
{

    qDebug() <<"disconneted";
    qDebug() << "error "  << socket->errorString();
}

void Smtp::connected()
{
    qDebug() << "Connected ";
}

void Smtp::readyRead()
{
	
     qDebug() <<"readyRead";
    // SMTP is line-oriented

    QString responseLine;
    do
    {
        responseLine = socket->readLine();
        response += responseLine;
    }
    while ( socket->canReadLine() && responseLine[3] != ' ' );

	responseLine.truncate( 3 );


    if ( state == Init && responseLine[0] == '2' )
    {
        // banner was okay, let's go on
		*t << "HELO there\r\n";
		t->flush();
		
        state = Mail;
        emit status( 3 ); //Sending
    }
    else if ( state == Mail && responseLine[0] == '2' )
    {
        // HELO response was okay (well, it has to be)

        *t << "MAIL FROM: " << from << "\r\n";
		t->flush();
        state = Rcpt;
        emit status( 4 ); //Sending
    }
    else if ( state == Rcpt && responseLine[0] == '2' )
    {
    	QString adress = rcpt[rcptIndex];
        *t << "RCPT TO: " << rcpt[rcptIndex] << "\r\n"; //r
        rcptIndex++;
		t->flush();
        emit status( 5 ); //Sending
        if (rcptIndex >= rcpt.size())
        	state = Data;
    }
    else if ( state == Data && responseLine[0] == '2' )
    {

        *t << "DATA\r\n";
		t->flush();
        state = Body;
        emit status( 6 ); //Sending
    }
    else if ( state == Body && responseLine[0] == '3' )
    {

        *t << message << "\r\n.\r\n";
		t->flush();
        state = Quit;
    }
    else if ( state == Quit && responseLine[0] == '2' )
    {

        *t << "QUIT\r\n";
		t->flush();
        // here, we just close.
        state = Close;
        emit status( 7 ); //Sending
    }
    else if ( state == Close )
    {
        deleteLater();
        return;
    }
    else
    {
        // something broke.
        QMessageBox::warning( 0, tr( "Mail error" ), tr( "Unexpected reply from SMTP server:\n\n" ) + response );
        emit status(-1);
        state = Close;
    }
    response = "";
}
