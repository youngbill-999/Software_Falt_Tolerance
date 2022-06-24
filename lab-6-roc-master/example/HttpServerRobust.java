

import tud.se.httpd.HttpConfig;

import tud.se.httpd.HttpServer;


public class HttpServerRobust extends HttpServer {
    public volatile boolean restart;
    public HttpServerRobust(HttpConfig arg0) {
        super(arg0);
        this.restart=false;
    }
    protected void afterHandlingRequest() {

       if(this.getReceivedRequests()==15)
       {
           this.restart =true;
          
       }
    }
    
}
