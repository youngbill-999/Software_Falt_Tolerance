import os
import sys
import threading
import tarfile
import subprocess
import sys
import shutil



def compare(content, temp):
    if not os.path.exists(temp):
        return "no_tar"
    contentfile = tarfile.open(content)
    try:
        tempfile = tarfile.open(temp)
    except Exception:
        return "empty"

    tempfile = tarfile.open(temp)
    if tempfile.getmembers() == []:
        return "empty"

    for data in contentfile.getmembers():
        contentextract = contentfile.extractfile(data.name)
        if not contentextract == None:
            content = contentextract.read()
            tempextract = tempfile.extractfile(data.name)
            if tempextract == None:
                return "corrupted"
            else:
                content1 = tempextract.read()
                if not content == content1:
                    return "corrupted"
    return "okay"

def TIMEOUT():
    ProcessState = "timeout"

if __name__ == "__main__":
    global Injected
    global ProcessState
    global TarState
    Injected = sys.argv[1]
    ProcessState = "exited"
    TarState = "corrupted"
    
    parameter='faultinjectors/fi_'+sys.argv[1]+'.so'
    os.environ['LD_PRELOAD']=parameter
    
    os.spawnl(os.P_WAIT, "/bin/rm", "rm", "-rf","temp.tar")
    os.spawnl(os.P_WAIT, "/bin/rm", "rm", "-rf","./content")
    file=tarfile.open('content.tar')
    file.extractall('.')


    

    execution = subprocess.Popen("tar cf temp.tar content", shell=True, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    
    execution.wait()
   
    status = execution.poll()
    if status == 0:
        ProcessState = "success"
    elif status < 0:
        ProcessState = "signaled"
    elif status > 0:
        ProcessState = "exited"
    else:
        threading.Timer(5, TIMEOUT).start()
        execution.kill()
    
    TarState = compare("content.tar", "temp.tar")

    print("Injected:", Injected)
    print("ProcessState:", ProcessState)
    print("TarState:", TarState)

    







