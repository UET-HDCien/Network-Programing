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
		System.out.print("Enter server IP, default port=1508: ");
		String IP=in.nextLine();
		try {
			sockfd=new Socket(IP,1508);
			System.out.println("Connected, press \"QUIT\" to close connection!");
				
			while(1==1) {
				DataInputStream dis = new DataInputStream(sockfd.getInputStream());
				DataOutputStream dos = new DataOutputStream(sockfd.getOutputStream());
				System.out.println("Enter file name: ");
				String fileName=in.nextLine();
				if (fileName.equals("QUIT")) break; 
				//sendFile(fileName);
				dos.write(fileName.getBytes());
				int size;
				//byte[] s=new byte[4];
				//dis.read(s);
				size=dis.readInt();
				System.out.println(size);
				if(size==0) {
					System.out.println("File not found!");
					continue;
				}
				FileOutputStream fos = new FileOutputStream(fileName.trim());
				byte[] buffer = new byte[1024];
				while (size>0) {
					num=dis.read(buffer);
					size-=num;
					fos.write(buffer, 0, num);
				}
				fos.close();
			}
			sockfd.close();
		}
		catch (Exception e) {
			System.out.println(e);
		}

	}
}
