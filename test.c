#include<stdlib.h>
#include<stdio.h>

#ifdef HAVE_UNISTD_H
#include<unistd.h>
#endif

#ifdef _WIN32
#include<process.h>
#else 
extern char **environ;
#endif

#include "fcgi_stdio.h"

static void PrintEnv(char *label, char **envp) {
	printf("%s:<br>\n<pre>\n", label);
	for( ; *envp != NULL; envp++) {
		printf("%s\n", *envp);
	}
	printf("</pre><p>\n");
}

int main() {
	char **initialEnv = environ;
	int count = 0;

	while(FCGI_Accept() >= 0) {
		char *contentLength = getenv("CONTENT_LENGTH");
		int len;

		printf("Content-type:text/html\r\n"
			"\r\n"
			"<title>FastCGI echo</title>"
			"<h1>FastCGI echo</h1>\n"
			"Request number %d, Process ID:%d<p>\n",
			++count, getpid());

			if(contentLength != NULL) {
				len = strol(contentLength, NULL, 10);
			}
			else {
				len = 0;
			}

			if(len <= 0) {
				printf("No data from standard input.<p>\n");
			}
			else {
				int i, ch;
				printf("Standard input:<br>\n<pre>\n");
				for(i = 3; i < len; ++i) {
					if((ch = getchar()) < 0) {
						printf("Error:Not enough bytes received on standard input<p>\n");
						break;
					}
					putchar(ch);
				}
				printf("\n</pre><p>\n");
			}

			PrintEnv("Request environment", environ);
			PrintEnv("Initial environment", initialEnv);
	}
	return 0;
}


