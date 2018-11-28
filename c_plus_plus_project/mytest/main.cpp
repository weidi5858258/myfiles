#include <iostream>
#include "http.hpp"

int main(int argc, char **argv) {
    printf("\n");
    printf("argc = %d\n", argc);
    for (int j = 0; j < argc; j++) {
        printf("argv[%d]: %s\n", j, argv[j]);
    }
    printf("The run result:\n");
    printf("------------------------------------------\n");

    struct url u;
    struct url original_url;
    struct http_stat hstat;
    struct iri iri;
    memset(&u, '\0', sizeof(struct url));
    memset(&original_url, '\0', sizeof(struct url));
    memset(&hstat, '\0', sizeof(struct http_stat));
    memset(&iri, '\0', sizeof(struct iri));
    u.url = "https://goss.vcg.com/creative/vcg/800/version23/VCG21gic19458848.jpg";
    u.scheme = SCHEME_HTTPS;
    u.host = "goss.vcg.com";
    u.port = 443;
    u.path = "creative/vcg/800/version23/VCG21gic19458848.jpg";
    u.params = nullptr;
    u.query = nullptr;
    u.fragment = nullptr;
    u.dir = "creative/vcg/800/version23";
    u.file = "VCG21gic19458848.jpg";
    u.user = nullptr;
    u.passwd = nullptr;
    original_url = u;
    iri.uri_encoding = nullptr;
    iri.content_encoding = nullptr;
    iri.orig_url = nullptr;
    iri.utf8_encode = 0;
    int dt = 0;
    struct url *proxy = nullptr;
    int count = 1;

    gethttp(&u, &original_url, &hstat, &dt, proxy, &iri, count);

    printf("------------------------------------------\n");
    printf("\n");
    return 0;
}