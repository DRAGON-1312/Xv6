#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void printIndent(int depth) {
for(int i = 0; i < depth; i++) {
printf(" ");
}
}

void tree(char *path, char *name, int depth) {
int fd;
struct stat st;
struct dirent de;
char buf[512], *p;

if((fd = open(path, 0)) < 0){
printf("tree: cannot open %s\n", path);
return;
}

if(fstat(fd, &st) < 0){
printf("tree: cannot stat %s\n", path);
close(fd);
return;
}

if(st.type != T_DIR){
printIndent(depth);
printf("%s\n", name);
close(fd);
return;
}

printIndent(depth);
printf("%s/\n", name);

if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
printf("tree: path too long\n");
close(fd);
return;
}

strcpy(buf, path);
p = buf + strlen(buf);
*p++ = '/';

while(read(fd, &de, sizeof(de)) == sizeof(de)){
if(de.inum == 0)
continue;

if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
continue;

memmove(p, de.name, DIRSIZ);
p[DIRSIZ] = 0;

struct stat st2;
if(stat(buf, &st2) < 0){
printf("tree: cannot stat %s\n", buf);
continue;
}

if(st2.type == T_DIR){
tree(buf, de.name, depth + 1);
} else {
printIndent(depth + 1);
printf("%s\n", de.name);
}
}

close(fd);
}

int main(int argc, char *argv[]) {
if(argc == 1){
tree(".", ".", 0);
} else {
tree(argv[1], argv[1], 0);
}
exit(0);
}
