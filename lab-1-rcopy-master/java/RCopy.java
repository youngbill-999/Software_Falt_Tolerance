import java.io.*;
public class RCopy {
    public static void main (String[] args) {
        try {
            int BUFFER_SIZE=1024;
            byte[] buffer = new byte [BUFFER_SIZE];
            int size=0;

            RandomAccessFile inFile = new RandomAccessFile(args[0], "r");
            FileOutputStream outStream = new FileOutputStream(args[1]);
           
            if(inFile.length()>BUFFER_SIZE){
            	size++;
            	inFile.seek(inFile.length()-BUFFER_SIZE*size);
            	
            }else{
            	size++;
            }
	        int read=inFile.read (buffer);
            while(read!=-1){
            	int start=0,end=read-1;
                byte Data;
                while(start<end) {
                	Data=buffer[start];
                	buffer[start]=buffer[end];
                	buffer[end]=Data;
                	start++;
                	end--;
                }
                outStream.write (buffer, 0, read);
                if((inFile.length()-BUFFER_SIZE*size)>BUFFER_SIZE){
                	size++;
                	inFile.seek(inFile.length()-BUFFER_SIZE*size);
                    read=inFile.read (buffer);
                }else if((inFile.length()-BUFFER_SIZE*size)>0){
                	inFile.seek(0);
                    read=inFile.read(buffer, 0, (int) (inFile.length()-BUFFER_SIZE*size));
                	size++;

                }else
                {
                	break;
                }
                
            }
            
            inFile.close ();
            outStream.close ();
        } catch (IOException e) {
            System.out.println ("IO-Exception while: " + e.toString());
        }
    }
}


