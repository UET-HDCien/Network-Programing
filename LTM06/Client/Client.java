import java.net.*;
import java.io.*;
import java.nio.*;
import java.io.File;
import java.io.IOException;
import java.util.Scanner;
public class Client {
	public static void main (String[] args) {
		Socket sockfd;
		int num;
		int remaining;
		Scanner in=new Scanner(System.in);
		System.out.println("Enter server IP, default port=1508: ");
		String IP=in.nextLine();
		try {
			sockfd=new Socket(IP,1508);
			System.out.println("Enter file name: ");
			String fileName=in.nextLine();
			//sendFile(fileName);
			DataInputStream dis = new DataInputStream(sockfd.getInputStream());
			DataOutputStream dos = new DataOutputStream(sockfd.getOutputStream());	
			dos.write(fileName.getBytes());
			int size;
			byte[] s=new byte[4];
			dis.read(s);
			size=(int)s[0]*256*256*256+(int)s[1]*256*256+(int)s[2]*256+(int)s[3];
			System.out.println(size);
			FileOutputStream fos = new FileOutputStream(fileName.trim());
			byte[] buffer = new byte[1024];
			while (size>0) {
				num=dis.read(buffer);
				size-=num;
				fos.write(buffer, 0, num);
			}
			fos.close();
			dis.close();
			dos.close();
		}
		catch (Exception e) {
			System.out.println(e);
		}
	}
}
