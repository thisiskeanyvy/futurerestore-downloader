#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h> // install libcurl

void getLatestVersion() {
	CURL *curl = curl_easy_init();

	if(!curl) {
		fprintf(stderr, "Curl initialization failed\n");
		exit(-1);
	}

	CURLcode res;
	curl_easy_setopt(curl, CURLOPT_URL, "https://github.com/futurerestore/futurerestore/actions");
	res = curl_easy_perform(curl);

	if(res != CURLE_OK) {
		fprintf(stderr, "Error retrieving url contentl\n");
		exit(-2);
	}

	curl_easy_cleanup(curl);
}

int main() {
	getLatestVersion();
	return 0;
}