import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

import tud.se.httpd.HttpConfig;
import tud.se.httpd.HttpException;
import tud.se.httpd.HttpServer;
import tud.se.httpd.HttpWorker;

/**
 * <p>Simple demonstration of how to compose the HttpServer and the HttpWorker
 * to a simple webserver.</p>
 *
 * @author <pre>part of: tiny-http package:
 * File: SimpleMain.java</pre>
 */
public class SimpleMain
{
	public static void main (String[] args) throws IOException, HttpException
	{
		// parse command line arguments
		HttpConfig config = new HttpConfig (args);
		// creating the worker 
		HttpWorkerRobust worker = new HttpWorkerRobust (config);
		// creating the server
		HttpServerRobust server = new HttpServerRobust (config);
		// connectiong the worker to the server
		server.setWorker (worker);
		// start the server
		server.start ();
		
		while(true){
			if(worker.restart==true)
			{
				worker = null;
				worker = new HttpWorkerRobust(config);
                server.setWorker(worker);
			}
			if(server.restart==true)
			{
				server.halt();
				server=null;
			
				server = new HttpServerRobust(config);
				server.setWorker(worker);
				server.start();
			}
		}

	}
}
