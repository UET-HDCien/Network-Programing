import java.net.*;
import java.io.*;
import java.nio.*;
import java.io.File;
import java.io.IOException;
class EchoThread extends Thread {
   	static final int BUFSIZE=1024; // define a constant used as size of buffer
   	public static int totalFile;
   	private Socket connSock;
   	private DataInputStream in;
   	private DataOutputStream out;
   	private final String lock = "aaaa";
   	public EchoThread(Socket connSock) {
    	this.connSock = connSock;
   	}
   	public synchronized void incTotal(){
   		totalFile++;
        System.out.println("Total file download:" +totalFile);
   	}
   public void run( )  {
    byte[] buff = new byte[BUFSIZE];
    byte[] buffer=new byte[BUFSIZE];
    String fileName;
    int totalByte;  //total byte of file
    int numByteRead; //number byte read fro socket
    int numByteRead2; //number byte read from file
    try {     
        in =new DataInputStream(connSock.getInputStream());
        out =new DataOutputStream(connSock.getOutputStream());
        //read/write loop
		while (1==1) {
			for (int i=0;i<BUFSIZE;i++) buff[i]=0x00;
			numByteRead = in.read(buff);
			if (numByteRead<=0) break;
            fileName=new String(buff);
            System.out.println("Client need file: "+fileName);
	    	File file=new File(fileName.trim());
            if (!file.exists() || !file.isFile()) {
                System.out.println("File Not found!");
                //ByteBuffer b = ByteBuffer.allocate(4);
                out.writeInt(0);
                continue;
            }
            FileInputStream ios = new FileInputStream(fileName.trim());
            totalByte=(int)file.length();
            System.out.println("File size is:"+totalByte);
            ByteBuffer b = ByteBuffer.allocate(4);
            b.putInt(totalByte);
            //out.write(b.array(), 0, 4);
            out.writeInt(totalByte);
            while ((numByteRead2 = ios.read(buffer)) != -1) {
                out.write(buffer, 0, numByteRead2);
				// s=new String(buffer);
                //System.out.println(s);
            }
            this.incTotal();
            
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
	//final static Object lock = new Object();
	//public int totalFile;
	
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
