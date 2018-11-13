//
// Created by root on 18-11-13.
//

#ifndef MYSTUDY_DOWNLOADFILE_HPP
#define MYSTUDY_DOWNLOADFILE_HPP

#include "./MyHead.h"

typedef string::size_type (string::*find_t)(const string &delim,
                                            string::size_type offset) const;

vector<string> Split(const string &s,
                     const string &match,
                     bool removeEmpty = false,
                     bool fullMatch = false) {
    vector<string> result;                 // return container for tokens
    string::size_type start = 0,           // starting position for searches
            skip = 1;            // positions to skip after a match
    find_t pfind = &string::find_first_of; // search algorithm for matches
    if (fullMatch) {
        skip = match.length();
        pfind = &string::find;
    }
    while (start != string::npos) {
        string::size_type end = (s.*pfind)(match, start);
        if (skip == 0) end = string::npos;
        string token = s.substr(start, end - start);
        if (!(removeEmpty && token.empty())) {
            result.push_back(token);
        }
        if ((start = end) != string::npos) start += skip;
    }
    return result;
}

void SplitProperty(vector<string> property, string *name, string *value) {
    vector<string>::iterator it = property.begin();
    if (it != property.end()) {
        name->clear();
        name->append(*it);
    }
    it++;
    if (it != property.end()) {
        value->clear();
        value->append(*it);
    }
}

int GetFileSize(char *host, char *file, string *error, int &headersize) {
    int size = -1;
    struct sockaddr_in servaddr;
    struct hostent *hp;
    string splitline = "\r\n";
    string PName;
    string PValue;
    string splittagbalue = ":";
    string info;
    vector<string> properties;
    vector<string> property;
    int sock_id;
    char message[1024 * 1024] = {0};
    int msglen;
    string request;
    request.append("HEAD ");
    request.append(file);
    request.append(" HTTP/1.1\n");
    request.append("Host:");
    request.append(host);
    request.append("\r\n\r\n");
    //Get a socket
    if ((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        error->append("Couldn't get a socket!");
        return size;
    }
    //book uses bzero which my man pages say is deprecated
    //the man page said to use memset instead. :-)
    memset(&servaddr, 0, sizeof(servaddr));
    //get address for google.com
    if ((hp = gethostbyname(host)) == NULL) {
        error->append("Couldn't access network.");
        error->append(host);
        return size;
    }
    //bcopy is deprecated also, using memcpy instead
    memcpy((char *) &servaddr.sin_addr.s_addr, (char *) hp->h_addr, hp->h_length);
    //fill int port number and type
    servaddr.sin_port = htons(80);
    servaddr.sin_family = AF_INET;
    //make the connection
    if (connect(sock_id, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
        error->append("Couldn't connect!");
        return size;
    }
    write(sock_id, request.c_str(), request.length());
    //read the response
    msglen = read(sock_id, message, 1024 * 1024);
    headersize = msglen;
    info.append(message, 0, msglen);
    close(sock_id);
    properties = Split(info, splitline, true);
    vector<string>::iterator it;
    for (it = properties.begin(); it < properties.end(); it++) {
        property = Split(*it, splittagbalue, true);
        SplitProperty(property, &PName, &PValue);
        if (PName == "Content-Length") {
            size = atoi(PValue.c_str());
            break;
        }
    }
    if (size == -1) {
        error->append("Resource Not Found!");
    }
    return size;
}

void DownloadFile(char *host, char *file, char *savefile, float size, int &progress, int hsize) {
    struct sockaddr_in servaddr;
    struct hostent *hp;
    string info;
    int sock_id;
    char message[18000] = {0};
    char messagetop[18000] = {0};
    int msglen;
    float readcount = 0;
    string request;
    request.append("GET ");
    request.append(file);
    request.append(" HTTP/1.1\n");
    request.append("Host:");
    request.append(host);
    request.append("\r\n\r\n");
    //Get a socket
    if ((sock_id = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        return;
    }
    //book uses bzero which my man pages say is deprecated
    //the man page said to use memset instead. :-)
    memset(&servaddr, 0, sizeof(servaddr));
    //get address for google.com
    if ((hp = gethostbyname(host)) == NULL) {
        return;
    }
    //bcopy is deprecated also, using memcpy instead
    memcpy((char *) &servaddr.sin_addr.s_addr, (char *) hp->h_addr, hp->h_length);
    //fill int port number and type
    servaddr.sin_port = htons(80);
    servaddr.sin_family = AF_INET;
    //make the connection
    if (connect(sock_id, (struct sockaddr *) &servaddr, sizeof(servaddr)) != 0) {
        return;
    }
    //NOW THE HTTP PART!!!
    //send the request
    write(sock_id, request.c_str(), request.length());
    ofstream outfile(savefile, ofstream::binary);
    do {
        msglen = read(sock_id, message, 18000);
        if (msglen == 0)
            break;
        if (readcount == 0) {
            int tempindex = 0;
            for (int i = hsize - 1; i < msglen; i++) {
                messagetop[tempindex] = message[i];
                tempindex = tempindex + 1;
            }
            outfile.write(messagetop, tempindex);
        } else {
            outfile.write(message, msglen);
        }
        readcount = readcount + msglen;
        progress = readcount / size * 100;
    } while (readcount <= (size + hsize));
    outfile.close();
    close(sock_id);
    //read the response
}

float filesize;
int loadprogress = 0;
int hsize;
pthread_t tUpdateWork;

void *UpdateWorCoping(void *data) {
    DownloadFile("192.168.2.128", "/games.data", "/usr/games.data", filesize, loadprogress, hsize);
}

int main2(int argc, char **argv) {
    string error;
    filesize = GetFileSize("192.168.2.128", "/games.data", &error, hsize);
    pthread_create(&tUpdateWork, NULL, UpdateWorCoping, 0);
    while (loadprogress < 99) {
        printf("Progress %d \n", loadprogress);
        usleep(1000000);
    }
    return 0;
}

#endif //MYSTUDY_DOWNLOADFILE_HPP
