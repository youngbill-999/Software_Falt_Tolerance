import tud.se.httpd.HttpConfig;
import tud.se.httpd.HttpWorker;

public class HttpWorkerRobust extends HttpWorker {
    public volatile boolean restart;
    public HttpWorkerRobust(HttpConfig arg0) {
        super(arg0);
        this.restart=false;
    }
    
    protected void afterProcessingRequest() {
        //System.out.println(this.getRequestNum());
        if(this.getRequestNum()==10)
        {
            this.restart=true;
        }
    }
   
    
}
