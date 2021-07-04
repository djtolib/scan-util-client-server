
#include <unistd.h>
#include "server.h"
string server::scan_dir(const string& dir) {
    //Timing
    auto begin = chrono::high_resolution_clock::now();
    stringstream out;
    filesystem::path path(dir);
    if(!is_directory(path)){
        out<<"Directory is not exists."<<endl;
        return out.str();
    }

    ifstream file;
    string line;
    int unix_suspicious = 0, js_suspicious = 0, mac_suspicious = 0, errors = 0;

    for(auto &p : filesystem::directory_iterator(path)) {
        if (!p.is_directory()) {
            file.open(p.path(), ios_base::in);
            if (file.is_open()) {
                while (getline(file, line)) {
                    if (line.find("rm -rf ~/Documents") != string::npos) {
                        ++unix_suspicious;
                        break;
                    }
                    if (line.find("system(\"launchctl load /Library/LaunchAgents/com.malware.agent\")") !=
                        string::npos) {
                        ++mac_suspicious;
                        break;
                    }
                    if (p.path().extension().string() == ".js" &&
                        line.find("<script>evil_script()</script>") != string::npos) {
                        ++js_suspicious;
                        break;
                    }
                }
                file.close();
            }
            else ++errors;
        }// end of not directory if
    }// end of for

    // Special emulation
    sleep(2);

    auto end = chrono::high_resolution_clock::now();

    // Chrono plays
    unsigned int secs = chrono::duration_cast<chrono::seconds>(end - begin).count() % 60;
    unsigned int mins = chrono::duration_cast<chrono::minutes>(end - begin).count()  % 60;
    unsigned int hours = chrono::duration_cast<chrono::hours>(end - begin).count() ;

    // Outputting
    out<<"====== Scan result ======"<<endl
        <<"Processed files: "<< unix_suspicious + mac_suspicious + js_suspicious + errors <<endl
        <<"JS detects: "<<js_suspicious<<endl
        <<"Unix detects: "<<unix_suspicious<<endl
        <<"macOS detects: "<<mac_suspicious<<endl
        <<"Errors: "<<errors<<endl
        <<"Execution time: "<<setfill('0')<<setw(2)<<hours<<":"<<setfill('0')<<setw(2)<<mins<<":"
        <<setfill('0')<<setw(2)<<secs<<endl
        <<"========================="<<endl;

    return out.str();
}

void server::run() {

    int sock, listener;
    struct sockaddr_in addr;
    char buf[1024];
    size_t bytes_read;

    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0){
        throw runtime_error("Unable to create socket.");
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(3425);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        close(listener);
        throw runtime_error("Unable to bind.");
    }
    cout<<"== Scan service is started =="<<endl;
    listen(listener, 1);

    while(true)
    {
        sock = accept(listener, nullptr, nullptr);
        if(sock < 0){
            throw runtime_error("Unable to accept.");
        }
        string dir, result;

        bytes_read = recv(sock, buf, 1024, 0);
        dir += buf;
        dir = dir.substr(0, dir.find('\r'));
        //if(dir.back() == '\r') dir.pop_back();

        result = scan_dir(dir);
        send(sock, result.c_str(), result.size(), 0);
        close(sock);
    }


}

