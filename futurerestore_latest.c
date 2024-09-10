#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h> // install libcurl
#include <regex.h>

struct MemoryStruct
{
	char* memory;
	size_t size;
};

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp) { // Write in MemoryStruct + realloc memory
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct*) userp;

	char *ptr = realloc(mem -> memory, mem -> size + realsize + 1);
	if(!ptr) {
		printf("Error (memory) : realloc returned NULL");
		return 0;
	}

	mem -> memory = ptr;
	memcpy(&(mem -> memory[mem -> size]), contents, realsize);
	mem -> size = mem -> size + realsize;
	mem -> memory[mem -> size] = 0;

	return realsize;
}

struct MemoryStruct getLatestVersion() { // GET request to github action page
	curl_global_init(CURL_GLOBAL_ALL);
	CURL *curl = curl_easy_init();

	struct MemoryStruct chunk;

	chunk.memory = malloc(1);
	chunk.size = 0;

	if(!curl) {
		fprintf(stderr, "Curl initialization failed\n");
		exit(-1);
	}

	CURLcode res;
	curl_easy_setopt(curl, CURLOPT_URL, "https://github.com/futurerestore/futurerestore/actions");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) &chunk);
	res = curl_easy_perform(curl);

	if(res != CURLE_OK) {
		fprintf(stderr, "Error retrieving url contentl\n");
		exit(-2);
	}

    curl_easy_cleanup(curl);
    curl_global_cleanup();

	return chunk;
}

void REQParser(struct MemoryStruct chunk) { // Extract content from request
	/* Debug */
    // printf("Length (bytes) : %lu\n", (unsigned long) chunk.size);
    // printf("%s\n", chunk.memory);

	/*
	 * REQParser use regex to find content
	 */

	regex_t regex;
    regcomp(&regex, "/futurerestore/futurerestore/actions/runs/[0-9]+", REG_EXTENDED);
    
    regmatch_t match;
    const char* cursor = chunk.memory;
    
    while (regexec(&regex, cursor, 1, &match, 0) == 0) {
        int begin = match.rm_so;
        int end = match.rm_eo;
        
        printf("Build : https://github.com%.*s\n", end - begin, cursor + begin);
        
        cursor = cursor + end;
    }
    
    regfree(&regex);
}

int main() {
	struct MemoryStruct result = getLatestVersion();
	REQParser(result);
	free(result.memory);
	return 0;
}