#include <pwd.h>
#include <stddef.h>
#include <string.h>


struct passwd *
getpwnam(const char * name) {
	struct passwd *ptr;

	setpwent();//反绕到文件的开始
	while((ptr = getpwent()) != NULL) {
		if (strcmp(name, ptr->pw_name) == 0 ) {
			break;
		}
	}

	endpwent();
	return (ptr);
}

int main() {

}
