import java.net.*;
import java.io.*;
import java.nio.*;
import java.io.File;
import java.io.IOException;
class EchoThread extends Thread {
   static final int BUFSIZE=1024; // define a constant used as size of buffer
   private Socket connSock;
   private InputStream in;
   private OutputStream out;
   
   public EchoThread(Socket connSock) {
    this.connSock = connSock;
   }

   public void run( )  {
    byte[] buff = new byte[BUFSIZE];
    byte[] buffer=new byte[BUFSIZE];
    String fileName;
    int totalByte;  //total byte of file
    int numByteRead; //number byte read fro socket
    int numByteRead2; //number byte read from file
    try {     
        in = connSock.getInputStream();
        out = connSock.getOutputStream();
        //read/write loop
	while ((numByteRead = in.read(buff)) != -1) {
            fileName=new String(buff);
            System.out.println("Client need file: "+fileName);
	    File file=new File(fileName.trim());
            if (!file.exists() || !file.isFile()) {
                System.out.println("Not found!");
                ByteBuffer b = ByteBuffer.allocate(4);
                out.write(b.array(),0,4);
            }
            FileInputStream ios = new FileInputStream(fileName.trim());
            totalByte=(int)file.length();
            System.out.println("Size:"+totalByte);
            ByteBuffer b = ByteBuffer.allocate(4);
            b.putInt(totalByte);
	    //b=Collections.reverse(Bytes.asList(b));
            out.write(b.array(), 0, 4);
            while ((numByteRead2 = ios.read(buffer)) != -1) {
                out.write(buffer, 0, numByteRead2);
		String s=new String(buffer);
                System.out.println(s);
            }
        }
    }
    catch (SocketException ex) {
       //output thread closed the socket
    }
    catch (IOException ex) {
        System.err.println(ex);
    }

    try {
      connSock.close();
    }
    catch (IOException ex) { 
    } 
  }
}
public class Server{
    public static void main(String[] args) {
        int port=1508;
        try {
        ServerSocket servSocket = new ServerSocket(port, 1);
        while (true) { 
            Socket connection = servSocket.accept( );
            System.out.println("Connection established with "  + connection);
            Thread echoThread = new EchoThread(connection);
            echoThread.start();
        // wait for echoThread to finish 
       }
    }

    catch (IOException ex) {
      ex.printStackTrace( );
    } 
    }
} 
